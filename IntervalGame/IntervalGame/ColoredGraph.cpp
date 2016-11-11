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
	std::queue<bool> Q;

	for(unsigned i = 0, j = 0; i < 32; i++) {
		if(i == position) {
			graph.insert(i, true);
			Q.push(true);
			coloring.insert(j, color);
			j++;
			continue;
		}
		if(graph[i] == true) {
			Q.push(false);
			j++;
		}
		else {
			if(Q.empty() && i > position) return;
			if(Q.front()) {
				graph.insert(i, false);
			}
			Q.pop();
		}
	}
}

std::ostream& operator<<(std::ostream & os, ColoredGraph & cgraph) {
	os << "Graph: " << unsigned(cgraph.graph) << " Coloring: " << long long unsigned(cgraph.coloring) << " Valid: " << cgraph.isValid() << std::endl;

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
