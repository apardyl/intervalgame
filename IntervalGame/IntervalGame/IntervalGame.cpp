// IntervalGame.cpp : Defines the entry point for the console application.
//

#include <algorithm>
#include <float.h>
#include <iostream>
#include <map>
#include <mutex>
#include <set>
//#include <unordered_map>

#include "ColoredGraph.h"
#include "ThreadPool.h"

unsigned depth = 0;
unsigned target_width = 0;

template<class Key, class T> 
struct Results {
  typedef std::map<Key, T> map_type;
  typedef typename map_type::size_type size_type;
  typedef typename map_type::key_type key_type;
  typedef typename map_type::mapped_type mapped_type;
  typedef std::function<void(key_type)> function_type;

  ThreadPool<void, key_type>& tp;
  function_type sched;

  Results(ThreadPool<void, key_type>& tp, function_type schedule) :
    tp(tp),
    sched(schedule) {}

  map_type map;
  std::map<Key, size_t> missing_count;
  std::map<Key, std::set<Key>> missing;
  std::mutex mutex;

  void clear() {
    std::unique_lock<std::mutex> lock(mutex);
    tp.clear();
    map.clear();
    missing_count.clear();
    missing.clear();
  }
  size_type size() {
    std::unique_lock<std::mutex> lock(mutex);
    return map.size();
  }
  size_type count(const key_type& key) {
    std::unique_lock<std::mutex> lock(mutex);
    return map.count(key);
  }
  mapped_type operator[](const key_type& key) {
    std::unique_lock<std::mutex> lock(mutex);
    return map[key];
  }
  void set(const key_type& key, mapped_type val) {
    std::unique_lock<std::mutex> lock(mutex);

//    std::cerr << "Calculated " << key << ": " << val << " ( " << missing[key].size() << ")" << std::endl;
//    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    map[key] = val;
    for ( auto cgraph : missing[key] ) {
      auto& m = missing_count[cgraph];
      if ( m <= 1 ) {
//        std::cerr << "Running " << cgraph << std::endl;
//        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        tp.push_back(sched, cgraph);
        missing_count.erase(cgraph);
      } else {
        m--;
      }
    }
    missing.erase(key);
  }
  void schedule(const key_type& key, const std::vector<key_type>& tasks) {
    std::unique_lock<std::mutex> lock(mutex);
    size_t count = 0;
//    std::cerr << "Scheduling for" << key << std::endl;
    for ( auto task : tasks ) {
      if ( map.count(task) == 0 ) {
        if ( missing[task].size() == 0 ) {
//          std::cerr << "  Scheduling " << task << std::endl;
//          std::this_thread::sleep_for(std::chrono::milliseconds(1000));
          tp.push_back(sched, task);
        } else {
//          std::cerr << "  Task " << task << " is already scheduled" << std::endl;
        }
        if ( missing[task].count(key) == 0 ) {
          count++;
          missing[task].insert(key);
        }
      }
    }
    if ( count > 0 ) {
//      std::cerr << " Task " << key << " has " << count << " missing subtasks" << std::endl;
      missing_count[key] = count;
    }
    else {
//      std::cerr << "Rerunning ready task " << key << std::endl;
      tp.push_back(sched, key);
    }
  }
};

void schedule(ColoredGraph);
ThreadPool<void, ColoredGraph> tp;
Results<ColoredGraph, double> gameResults(tp, schedule);

double score(ColoredGraph cgraph) {
  if ( target_width > 0 ) {
    if ( cgraph.minColors() <= target_width )
    	return double(cgraph.colors()) / target_width;
    return 1;
  }
  return double(cgraph.colors()) / cgraph.minColors();
}

void schedule(ColoredGraph cgraph) {
	if (gameResults.count(cgraph) == 1) return;

	unsigned colors = cgraph.colors();

	if(cgraph.popcount() >= depth) {
	  gameResults.set(cgraph, score(cgraph));
	  return;
	}

	double max = -DBL_MAX;

  std::vector<ColoredGraph> missing;

	for(unsigned i = 0; i <= cgraph.popcount() * 2 + 1;i++) {
		bool stop = false;
		for (unsigned k = 0; k <= cgraph.popcount() && !stop; k++) {
			double min = DBL_MAX;
			for (unsigned j = 1; j <= colors + 1; j++) {
				ColoredGraph ncgraph = cgraph;
				if (!ncgraph.insert(i, k, j)) {
					stop = true;
					break;
				}

				if (!ncgraph.isValid()) continue;
				ncgraph.normalize();

				double x = DBL_MAX;
				if (gameResults.count(ncgraph) == 1) x = gameResults[ncgraph];
				else missing.emplace_back(ncgraph);

				min = std::min(min, x);
			}
			if (min != DBL_MAX) max = std::max(max, min);
		}
	}

  max = std::max(max, score(cgraph));

  if ( missing.size() == 0 )
    gameResults.set(cgraph, max);
  else
    gameResults.schedule(cgraph, missing);
	//std::cout << cgraph << max << std::endl;
}

double result(ColoredGraph cgraph) {
//  std::cout << "Asking for " << cgraph << std::endl;
  tp.push_back(schedule, cgraph);
  while (true) {
    if (gameResults.count(cgraph) == 1)
      return gameResults[cgraph];
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
}

int main() {

	for (int i = 1; i <=16;i++) {
		gameResults.clear();
		depth = i;
		ColoredGraph graph;
		double x = result(graph);
		std::cout << i << ": k: " << x  << " mapSize: " << gameResults.size() << std::endl;
	}

	std::cin.get();

    return 0;
}
