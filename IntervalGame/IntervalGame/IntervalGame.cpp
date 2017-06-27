// IntervalGame.cpp : Defines the entry point for the console application.
//

#include "IntervalGraph.h"
#include "ThreadPool.h"
#include "GraphMap.h"

#include <algorithm>
#include <float.h>
#include <iostream>
#include <mutex>
#include <unordered_set>


const unsigned TARGET_WIDTH = 0;
const unsigned MAX_NESTED = 0;
const bool SHOW_FULL_PATH = false;
const bool INTERACTIVE = false;

//#define DEBUG
//#define MULTITHREADING_DEBUG

unsigned depth = 0;

template <class T>
struct Results {
	typedef IntervalGraph key_type;
	typedef std::function<void(IntervalGraph)> function_type;

	ThreadPool<void, IntervalGraph> &tp;
	function_type sched;

	Results(ThreadPool<void, IntervalGraph> &tp, function_type schedule) :
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
#ifdef MULTITHREADING_DEBUG
		std::unique_lock<std::mutex> lock(mutex);
		std::cerr << "Calculated " << key << ": " << val << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
#endif
		resultMap[key] = val;
	}

	void schedule(const key_type &key, const std::unordered_set<IntervalGraph> &tasks) {
#ifdef MULTITHREADING_DEBUG
		std::unique_lock<std::mutex> lock(mutex);
		    std::cerr << "Scheduling for" << key << std::endl;
#endif
		for(auto task : tasks) {
			if(resultMap.count(task) == 0) {
#ifdef MULTITHREADING_DEBUG
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

void schedule(IntervalGraph);
#ifdef DEBUG
ThreadPool<void, IntervalGraph> tp(1);
#else
ThreadPool<void, IntervalGraph> tp;
#endif

struct Outcome {
	double score;
	unsigned short bestNext;

	Outcome(double score = -1, int start = 0, int length = 0, int color = 0)
		: score(score), bestNext(start << 8 | length << 4 | color) {
	}

	bool operator<(const Outcome &o) const {
		return score < o.score || (score == o.score && bestNext < o.bestNext);
	}

	void setBest(int start, int length, int color) {
		bestNext = start << 8 | length << 4 | color;
	}

	bool hasNext() const {
		return bestNext != 0;
	}

	int getStart() const {
		return (bestNext & (0xF << 8)) >> 8;
	}

	int getLength() const {
		return (bestNext & (0xF << 4)) >> 4;
	}

	int getColor() const {
		return bestNext & 0xF;
	}

	friend std::ostream& operator<<(std::ostream &os, const Outcome o) {
		os << o.score << std::endl;
		return os;
	}
};

Results<Outcome> gameResults(tp, schedule);

double score(IntervalGraph cgraph) {
	if(TARGET_WIDTH > 0) {
		if(cgraph.minColors() <= TARGET_WIDTH)
			return double(cgraph.colors()) / TARGET_WIDTH;
		return 1;
	}
	return double(cgraph.colors()) / cgraph.minColors();
}

void schedule(IntervalGraph cgraph) {
	if(gameResults.count(cgraph) == 1) return;

	unsigned colors = cgraph.colors();

	if(cgraph.length() >= depth) {
		gameResults.set(cgraph, Outcome(score(cgraph)));
		return;
	}

	Outcome max = Outcome(-DBL_MAX);

	std::unordered_set<IntervalGraph> missing;
	int missingCount = 0;

	unsigned cgraphLength = cgraph.length();
	for(unsigned i = 0; i <= cgraphLength * 2; i++) {
		for(unsigned k = 0; k <= cgraphLength * 2 - i; k++) {
			Outcome min = Outcome(DBL_MAX);
			for(unsigned j = 1; j <= colors + 1; j++) {
				IntervalGraph ncgraph = cgraph;
				if(!ncgraph.insert(i, k, j)) continue;

				if(!ncgraph.isValid(MAX_NESTED)) break;
				ncgraph.normalize();

				Outcome x = Outcome(DBL_MAX);
				if(gameResults.count(ncgraph) == 1) x = gameResults[ncgraph];
				else {
					if(!gameResults.scheduledMap[ncgraph]) missing.emplace(ncgraph);
					missingCount++;
				}

				x.setBest(i, k, j);
				min = std::min(min, x);
			}
			if(min.score != DBL_MAX) max = std::max(max, min);
		}
	}

	max = std::max(max, Outcome(score(cgraph)));

	if(missingCount == 0)
		gameResults.set(cgraph, max);
	else {
		gameResults.schedule(cgraph, missing);
	}
	//std::cout << cgraph << max << std::endl;
}

Outcome result(IntervalGraph cgraph) {
	//  std::cout << "Asking for " << cgraph << std::endl;
	tp.push_back(schedule, cgraph);
	while(true) {
		if(gameResults.count(cgraph) == 1)
			return gameResults[cgraph];
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

IntervalGraph nextInPath(const IntervalGraph &a, const Outcome &o) { //Should be an iterator
	IntervalGraph b = a;
	b.insert(o.getStart(), o.getLength(), o.getColor());
	b.normalize();
	return b;
}

void printWinner(IntervalGraph a) {
	while(true) {
		Outcome o = gameResults.resultMap[a];
		if(o.hasNext()) a = nextInPath(a, o);
		else break;
	}
	std::cout << a;
}

void printPath(IntervalGraph a) {
	while(true) {
		std::cout << a;
		Outcome o = gameResults.resultMap[a];
		if(o.hasNext()) a = nextInPath(a, o);
		else break;
	}
}

void graphUI() {
	while(true) {
		IntervalGraph graph;
		try {
			std::cin >> graph;
		} catch(std::logic_error) {
			break;
		}
		if(!graph.isValid(MAX_NESTED)) {
			std::cout << "Invalid graph" << std::endl;
			continue;
		}
		graph.normalize();
		if(SHOW_FULL_PATH) printPath(graph);
		else printWinner(graph);
	}
}

int main() {
	for(int i = 1; i <= 16; i++) {
		gameResults.clear();
		depth = i;
		IntervalGraph graph;
		Outcome x = result(graph);
		std::cout << i << ": k: " << x.score << std::endl;

		if(SHOW_FULL_PATH) printPath(graph);
		else printWinner(graph);
		gameResults.resultMap.statistics();
		if(INTERACTIVE) {
			graphUI();
		}
#ifdef DEBUG
		gameResults.resultMap.printContent();
		std::cin.get();
#endif
	}

	std::cin.get();

	return 0;
}
