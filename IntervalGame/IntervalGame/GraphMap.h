#pragma once

#include "IntervalGraph.h"
#include <map>
#include <mutex>
#include <iostream>

template <typename T, int hashtableSize = 1466329, typename container = std::map<IntervalGraph, T>>
class CGraphMap {
private:
	container map[hashtableSize];
	std::mutex locks[hashtableSize];
public:
	T& operator[](const IntervalGraph &c);
	size_t size();
	size_t count(const IntervalGraph &c);
	void clear();
	void statistics();
	void printContent();
	static unsigned hash(const IntervalGraph&c);
};

template <typename T, int hashtableSize, typename container>
T& CGraphMap<T, hashtableSize, container>::operator[](const IntervalGraph &c) {
	std::unique_lock<std::mutex> lock(locks[hash(c)]);
	return map[hash(c)][c];
}

template <typename T, int hashtableSize, typename container>
size_t CGraphMap<T, hashtableSize, container>::size() {
	size_t s = 0;
	for(int i = hashtableSize - 1; i >= 0; --i) {
		std::unique_lock<std::mutex> lock(locks[i]);
		s += map[i].size();
	}
	return s;
}

template <typename T, int hashtableSize, typename container>
size_t CGraphMap<T, hashtableSize, container>::count(const IntervalGraph &c) {
	std::unique_lock<std::mutex> lock(locks[hash(c)]);
	return map[hash(c)].count(c);
}

template <typename T, int hashtableSize, typename container>
void CGraphMap<T, hashtableSize, container>::clear() {
	for(int i = hashtableSize - 1; i >= 0; --i) map[i].clear();
}

template <typename T, int hashtableSize, typename container>
void CGraphMap<T, hashtableSize, container>::statistics() {
	size_t s = 0, zeros = 0;
	int max = 0;
	for(int i = hashtableSize - 1; i >= 0; --i) {
		std::unique_lock<std::mutex> lock(locks[i]);
		s += map[i].size();
		if(map[i].size() == 0) zeros++;
		if(map[i].size() > max) max =static_cast<int>(map[i].size());
	}
	std::cout << "size: " << s << " zeros: " << zeros << " maxLoad: " << max << std::endl;
}

template <typename T, int hashtableSize, typename container>
void CGraphMap<T, hashtableSize, container>::printContent() {
	for(int i = 0; i < hashtableSize; ++i) {
		for(auto a : map[i]) {
			std::cout << "graph: " << a.first << "score: " << a.second << std::endl;
		}
	}
}


template <typename T, int hashtableSize, typename container>
unsigned CGraphMap<T, hashtableSize, container>::hash(const IntervalGraph &c) {
	return static_cast<unsigned>(std::hash<IntervalGraph>{}(c) % hashtableSize);
}
