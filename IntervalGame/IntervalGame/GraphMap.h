#pragma once

#include "ColoredGraph.h"
#include <unordered_map>
#include <mutex>
#include <iostream>

template <typename T, typename container = std::unordered_map<ColoredGraph, T, graphMapHash>>
class CGraphMap {
private:
	container map[4194304];
	std::mutex locks[4194304];
public:
	T& operator[](const ColoredGraph &c);
	size_t size();
	size_t count(const ColoredGraph &c);
	void clear();
	void statistics();
	static unsigned hash(const ColoredGraph&c);
};

template <typename T, typename container>
T& CGraphMap<T, container>::operator[](const ColoredGraph &c) {
	std::unique_lock<std::mutex> lock(locks[hash(c)]);
	return map[hash(c)][c];
}

template <typename T, typename container>
size_t CGraphMap<T, container>::size() {
	size_t s = 0;
	for(int i = 4194303; i >= 0; i--) {
		std::unique_lock<std::mutex> lock(locks[i]);
		s += map[i].size();
	}
	return s;
}

template <typename T, typename container>
size_t CGraphMap<T, container>::count(const ColoredGraph &c) {
	std::unique_lock<std::mutex> lock(locks[hash(c)]);
	return map[hash(c)].count(c);
}

template <typename T, typename container>
void CGraphMap<T, container>::clear() {
	for(int i = 4194303; i >= 0; i--) map[i].clear();
}

template <typename T, typename container>
void CGraphMap<T, container>::statistics() {
	size_t s = 0, zeros = 0;
	int max = 0;
	for(int i = 4194303; i >= 0; i--) {
		std::unique_lock<std::mutex> lock(locks[i]);
		s += map[i].size();
		if(map[i].size() == 0) zeros++;
		if(map[i].size() > max) max = map[i].size();
	}
	std::cout << "size: " << s << " zeros: " << zeros << " maxLoad: " << max << std::endl;
}

template <typename T, typename container>
unsigned CGraphMap<T, container>::hash(const ColoredGraph &c) {
	return ((c.rawGraph() & 0x1FFFE) << 5) | ((c.colors() & 0xF) << 2) | (c.shortColorHash() & 0x3);
}
