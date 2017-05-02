// IntervalGame.cpp : Defines the entry point for the console application.
//

#include <algorithm>
#include <float.h>
#include <iostream>
#include <mutex>
#include <unordered_set>

#include "ColoredGraph.h"
#include "ThreadPool.h"
#include "GraphMap.h"

unsigned depth = 0;
const unsigned target_width = 0;

//#define DEBUG

template <class T>
struct Results {
	typedef ColoredGraph key_type;
	typedef std::function<void(ColoredGraph)> function_type;

	ThreadPool<void, ColoredGraph> &tp;
	function_type sched;

	Results(ThreadPool<void, ColoredGraph> &tp, function_type schedule) :
		tp(tp),
		sched(schedule) {
	}

	CGraphMap<T> resultMap;
	CGraphMap<bool> scheduledMap;

	std::mutex mutex;

	void clear() {
		std::unique_lock<std::mutex> lock(mutex);
		tp.clear();
		resultMap.clear(); 
		scheduledMap.clear();
	}

	size_t size() {
		return resultMap.size();
	}

	size_t count(const key_type &key) {
		return resultMap.count(key);
	}

	T operator[](const key_type &key) {
		return resultMap[key];
	}

	void set(const key_type &key, T val) {
#ifdef DEBUG
		std::unique_lock<std::mutex> lock(mutex);
		std::cerr << "Calculated " << key << ": " << val << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
#endif
		resultMap[key] = val;
	}

	void schedule(const key_type &key, const std::unordered_set<ColoredGraph> &tasks) {
#ifdef DEBUG
		std::unique_lock<std::mutex> lock(mutex);
		    std::cerr << "Scheduling for" << key << std::endl;
#endif
		for(auto task : tasks) {
			if(resultMap.count(task) == 0) {
#ifdef DEBUG
				std::cerr << "  Scheduling " << task << std::endl;
				std::this_thread::sleep_for(std::chrono::milliseconds(1000));
#endif
				scheduledMap[task] = true;
				tp.push_back(sched, task);
			}
		}
		tp.push_back(sched, key);
	}
};

void schedule(ColoredGraph);
ThreadPool<void, ColoredGraph> tp;
Results<double> gameResults(tp, schedule);

double score(ColoredGraph cgraph) {
	if(target_width > 0) {
		if(cgraph.minColors() <= target_width)
			return double(cgraph.colors()) / target_width;
		return 1;
	}
	return double(cgraph.colors()) / cgraph.minColors();
}

void schedule(ColoredGraph cgraph) {
	if(gameResults.count(cgraph) == 1) return;

	unsigned colors = cgraph.colors();

	if(cgraph.popcount() >= depth) {
		gameResults.set(cgraph, score(cgraph));
		return;
	}

	double max = -DBL_MAX;

	std::unordered_set<ColoredGraph> missing;
	int missingCount = 0;

	for(unsigned i = 0; i <= cgraph.popcount() * 2 + 1; i++) {
		bool stop = false;
		for(unsigned k = 0; k <= cgraph.popcount() && !stop; k++) {
			double min = DBL_MAX;
			for(unsigned j = 1; j <= colors + 1; j++) {
				ColoredGraph ncgraph = cgraph;
				if(!ncgraph.insert(i, k, j)) {
					stop = true;
					break;
				}

				if(!ncgraph.isValid()) continue;
				ncgraph.normalize();

				double x = DBL_MAX;
				if(gameResults.count(ncgraph) == 1) x = gameResults[ncgraph];
				else {
					if(!gameResults.scheduledMap[ncgraph]) missing.emplace(ncgraph);
					missingCount++;
				}

				min = std::min(min, x);
			}
			if(min != DBL_MAX) max = std::max(max, min);
		}
	}

	max = std::max(max, score(cgraph));

	if(missingCount == 0)
		gameResults.set(cgraph, max);
	else {
		gameResults.schedule(cgraph, missing);
	}
	//std::cout << cgraph << max << std::endl;
}

double result(ColoredGraph cgraph) {
	//  std::cout << "Asking for " << cgraph << std::endl;
	tp.push_back(schedule, cgraph);
	while(true) {
		if(gameResults.count(cgraph) == 1)
			return gameResults[cgraph];
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

int main() {

	for(int i = 1; i <= 16; i++) {
		gameResults.clear();
		depth = i;
		ColoredGraph graph;
		double x = result(graph);
		std::cout << i << ": k: " << x << std::endl;
		gameResults.resultMap.statistics();
	}

	std::cin.get();

	return 0;
}
