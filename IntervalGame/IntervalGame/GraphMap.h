#pragma once

#include "ColoredGraph.h"
#include <unordered_map>
#include <mutex>

template <typename T, typename container = std::unordered_map<ColoredGraph, T>>
class CGraphMap {
private:
	container map[1048576];
	std::mutex locks[1048576];
public:
	T& operator[](const ColoredGraph &c);
	size_t size();
	size_t count(const ColoredGraph &c);
	void clear();
};

template <typename T, typename container>
T& CGraphMap<T, container>::operator[](const ColoredGraph &c) {
	std::unique_lock<std::mutex> lock(locks[c.rawGraph() & 0xFFFFF]);
	return map[c.rawGraph() & 0xFFFFF][c];
}

template <typename T, typename container>
size_t CGraphMap<T, container>::size() {
	size_t s = 0;
	for(int i = 1048575; i >= 0; i--) {
		std::unique_lock<std::mutex> lock(locks[i]);
		s += map[i].size();
	}
	return s;
}

template <typename T, typename container>
size_t CGraphMap<T, container>::count(const ColoredGraph &c) {
	std::unique_lock<std::mutex> lock(locks[c.rawGraph() & 0xFFFFF]);
	return map[c.rawGraph() & 0xFFFFF].count(c);
}

template <typename T, typename container>
void CGraphMap<T, container>::clear() {
	for(int i = 1048575; i >= 0; i--) map[i].clear();
}
