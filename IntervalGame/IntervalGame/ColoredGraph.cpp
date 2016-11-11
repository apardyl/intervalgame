#include "ColoredGraph.h"
#include <queue>

ColoredGraph::ColoredGraph() {
}

bool ColoredGraph::isValid() {
	if(!graph.isValid()) return false;
	if(!coloring.isValid()) return false;

	std::queue<int> Q;
	bool colorInUse[16] = {true, false};

	for(int i = 0, j = 0; i < 32; i++) {
		if(graph[i] == true) {
			if(colorInUse[coloring[j]]) return false;
			colorInUse[coloring[j]] = true;
			Q.push(coloring[j]);
			j++;
		}
		else {
			if(Q.empty()) return true;
			if(!colorInUse[Q.front()]) return false;
			colorInUse[Q.front()] = false;
			Q.pop();
		}
	}

	return false;
}

void ColoredGraph::normailze() {
	Graph tmp = graph;
	tmp.reverse();
	if(unsigned(tmp) < unsigned(graph)) {
		graph = tmp;
		coloring.reverse();
	}

	coloring.normalize();
}

void ColoredGraph::insert(unsigned position, unsigned color) {
	int q = 1000;
	int p = 0;

	for(unsigned i = 0, j = 0; i < 32 && i <= position; i++) {
		if(i == position) {
			graph.insert(i, true);
			q = p;
			coloring.insert(j, color);
			j++;
			continue;
		}
		if(graph[i] == true) {
			p++;
			j++;
		}
		else {
			if(q == 0) {
				graph.insert(i, false);
			}
			q--;
		}
	}
}

bool ColoredGraph::operator==(const ColoredGraph & a) const {
	return (graph == a.graph) && (coloring == a.coloring);
}

std::ostream& operator<<(std::ostream & os, ColoredGraph & cgraph) {
	os << "Graph: " << unsigned(cgraph.graph) << " Coloring: " << long long unsigned(cgraph.coloring)
		<< " Valid: " << cgraph.isValid() << " MinColors: " << cgraph.graph.minColors() << std::endl;

	int col = cgraph.coloring.colors();
	for(int i = 1; i <= col; i++) {
		std::queue<int> Q;
		bool colorInUse = false;

		for(int k = 0, j = 0; k < 32; k++) {
			if(cgraph.graph[k] == true) {
				if(cgraph.coloring[j] == i) colorInUse = true;
				Q.push(cgraph.coloring[j]);
				j++;
			}
			else {
				if(Q.empty()) break;
				if(Q.front() == i) colorInUse = false;
				Q.pop();
			}
			if(colorInUse) os << "=";
			else os << " ";
		}
		os << std::endl;
	}

	return os;
}

size_t std::hash<ColoredGraph>::operator()(const ColoredGraph & a) const {
	return unsigned long long(a.coloring) + (unsigned long long(unsigned(a.graph)) << 32);
}