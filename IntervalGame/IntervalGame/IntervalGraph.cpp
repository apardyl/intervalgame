#include "IntervalGraph.h"
#include <set>
#include <utility>
#include <stdexcept>

int IntervalGraph::normailzeColors() {
	unsigned perm[16] = {0};
	unsigned next = 1;
	for(int i = 0; i < 32; i++) {
		if(graph[i] == 0) break;
		if(perm[graph[i]] == 0) {
			perm[graph[i]] = next++;
		}
		graph[i] = perm[graph[i]];
	}
	return next - 1;
}

void IntervalGraph::reverse() {
	int last = -1;
	for(; last < 32 && graph[last + 1] != 0; last++) {
	}
	for(int i = 0; i < last; i++ , last--) {
		int t = graph[i];
		graph[i] = static_cast<int>(graph[last]);
		graph[last] = t;
	}
	normailzeColors();
}

bool IntervalGraph::isValid(int maxNested) const {
	std::set<std::pair<int, int>> intervals;

	int interr[16];
	for(int i = 0; i < 16; i++) interr[i] = -1;
	for(int i = 0; i < 32 && graph[i] != 0; i++) {
		if(interr[graph[i]] == -1) {
			interr[graph[i]] = i;
		}
		else {
			intervals.emplace(interr[graph[i]], i);
			interr[graph[i]] = -1;
		}
	}

	for(auto i = intervals.begin(); i != intervals.end(); ++i) {
		std::set<std::pair<int, int>> nested;
		auto j = i;
		++j;
		for(; j != intervals.end() && j->first < i->second; ++j) {
			if(j->second < i->second) {
				nested.insert(*j);
				if(maxNested == 0) {
					return false;
				}
			}
		}
		int nst = 0;
		int last = 9999;
		for(auto a = nested.rbegin(); a != nested.rend(); ++a) {
			if(a->second < last) {
				last = a->first;
				nst++;
				if(nst > maxNested) {
					return false;
				}
			}
		}
	}

	return true;
}

void IntervalGraph::normalize() {
	normailzeColors();
	IntervalGraph tmp = *this;
	tmp.reverse();
	if(tmp < *this) this->graph = tmp.graph;
}

bool IntervalGraph::insert(unsigned start, unsigned length, unsigned color) {
	unsigned l = this->length();
	if(start > l*2) throw std::out_of_range("start index too far");
	if(start + length > l*2) throw std::out_of_range("inteval too long");

	graph.insert(start, 15);
	graph.insert(start + length + 1, 15);

	bool colors[16] = {false};
	for(unsigned i = 0; i < start; i++) {
		colors[graph[i]] ^= 1;
	}
	for(unsigned i = start; i < start + length + 1; i++) {
		colors[graph[i]] ^= 1;
		if(colors[color]) {
			return false;
		}
	}
	graph[start] = color;
	graph[start + length + 1] = color;
	return true;
}

unsigned IntervalGraph::length() const {
	int last = 0;
	for(; last < 32 && graph[last] != 0; last++) {
	}
	return last / 2;
}

unsigned IntervalGraph::colors() const {
	unsigned colors = 0;
	for(unsigned last = 0; last < 32 && graph[last] != 0; last++) {
		colors = (graph[last] > colors) ? graph[last] : colors;
	}
	return colors;
}

unsigned IntervalGraph::minColors() const {
	bool colors[16] = {false};
	int used = 0;
	int free = 0;
	for(int i = 0; i < 32; i++) {
		if(graph[i] == 0) break;
		colors[graph[i]] ^= 1;
		free = colors[graph[i]] ? free - 1 : free + 1;
		if(free < 0) {
			used += -free;
			free = 0;
		}
	}
	return used;
}

bool IntervalGraph::operator==(const IntervalGraph &a) const {
	return graph == a.graph;
}

bool IntervalGraph::operator<(const IntervalGraph &a) const {
	return graph < a.graph;
}

size_t std::hash<IntervalGraph>::operator()(const IntervalGraph &a) const {
	return std::hash<Array4Bit>{}(a.graph);
}

std::ostream& operator<<(std::ostream &os, const IntervalGraph &igraph) {
	unsigned col = igraph.colors();
	os << "Colors: " << col << " MinColors: " << igraph.minColors() << std::endl;

	for(unsigned i = 1; i <= col; i++) {
		bool colorInUse = false;

		for(int j = 0; j < 32 && igraph.graph[j] != 0; j++) {
			if(igraph.graph[j] == i) {
				os << '+';
				colorInUse ^= 1;
			} else {
				if(colorInUse) os << '=';
				else os << ' ';
			}
		}
		os << std::endl;
	}

	return os;
}
