#pragma once

#include <condition_variable>
#include <cstddef>
#include <functional>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <tuple>
#include <type_traits>
#include <utility>

template<class Result, class ...Args>

class ThreadPool {
  typedef std::function<Result(Args...)> function;
  typedef std::tuple<Args...> args;
  typedef std::pair<function, args> job;

  template<size_t N, class ...Params>
  struct Run {
    static inline Result run(function &&f, args &&a, Params &&...p) {
      return Run<N-1, typename std::tuple_element<N-1, args>::type, Params...>::run(std::forward<function>(f), std::forward<args>(a), std::forward<Params>(p)..., std::forward<typename std::tuple_element<N-1, args>::type>(std::get<N-1>(a)));
    }
  };
  template<class ...Params>
  struct Run<0, Params...> {
    static inline Result run(function &&f, args &&, Params &&...p) {
      return f(std::forward<Params>(p)...);
    }
  };

  static inline Result run(job j) {
    return Run<std::tuple_size<args>::value>::run(std::forward<function>(j.first), std::forward<args>(j.second));
  }

  bool shutdown;
  size_t length;
  std::deque<job> jobs;
  std::mutex mutex;
  std::condition_variable cv_consumer;
  std::condition_variable cv_producer;

  struct Worker {
    ThreadPool& tp;
    Worker(ThreadPool& tp) : tp(tp) {}
    void operator()() {
      while(true) {
        std::unique_ptr<job> j;
        {
          std::unique_lock<std::mutex> lock(tp.mutex);
          tp.cv_consumer.wait(lock, [this](){return tp.shutdown || !tp.jobs.empty();});
          if (tp.shutdown)
            return;
          j = std::unique_ptr<job>(new job(tp.jobs.front()));
          tp.jobs.pop_front();
          tp.cv_producer.notify_one();
        }
        run(*j);
      }
    }
  };
  std::vector<std::thread> workers;

  public:
  ThreadPool(size_t concurency = 0, size_t length = 0) : shutdown(false), length(length) {
	  if(concurency == 0)
		  concurency = std::thread::hardware_concurrency();
    for(size_t i=0; i<concurency; i++)
      workers.push_back(std::thread(Worker(*this)));
  }
  ~ThreadPool() {
    shutdown = true;
    cv_consumer.notify_all();
    for(auto& worker : workers)
      worker.join();
  }
  void clear() {
    shutdown = true;
    cv_consumer.notify_all();
    for(auto& worker : workers)
      worker.join();
    size_t concurency = workers.size();
    workers.clear();
    shutdown = false;
    for(size_t i=0; i<concurency; i++)
      workers.push_back(std::thread(Worker(*this)));
  }

  void push_back(function f, args a) {
    std::unique_lock<std::mutex> lock(mutex);
    if (length > 0)
      cv_producer.wait(lock, [this](){return shutdown || jobs.size() < length;});
    if (shutdown)
      return;
    jobs.emplace_back(f, a);
    cv_consumer.notify_one();
  }
  void push_back(function f, Args... a) {
    push_back(f, std::make_tuple(a...));
  }

};
