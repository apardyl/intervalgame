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

void ColoredGraph::normalize() {
	Graph tmp = graph;
	tmp.reverse();
	if(unsigned(tmp) < unsigned(graph)) {
		graph = tmp;
		coloring.reverse();
	}

	coloring.normalize();
}

bool ColoredGraph::insert(unsigned position, unsigned end, unsigned color) {
	int open = 0;
	unsigned i = 0, j = 0;

	for (; i < 32; i++) {
		if(i==position) {
			graph.insert(i, true);
			coloring.insert(j, color);
			j++;
			i++;
			break;
		}
		if(graph[i] == true) {
			open++;
			j++;
		}
		else {
			open--;
		}
	}
	for(;i<32 && open > 0;i++) {
		if(graph[i]==0) {
			open--;
		}
	}
	for(;i<32;i++) {
		if(end == 0) {
			graph.insert(i, false);
			return true;
		}
		if (graph[i] == false) return false;
		end--;
	}
	return false;
}

unsigned ColoredGraph::popcount() const {
	return graph.popcount();
}

unsigned ColoredGraph::colors() {
	return coloring.colors();
}

unsigned ColoredGraph::minColors() {
	return graph.minColors();
}

bool ColoredGraph::operator==(const ColoredGraph & a) const {
	return (graph == a.graph) && (coloring == a.coloring);
}

bool ColoredGraph::operator<(const ColoredGraph & a) const {
	if (unsigned(this->graph) != unsigned(a.graph)) return unsigned(this->graph) < unsigned(a.graph);
	return static_cast<unsigned long long>(this->coloring) < static_cast<unsigned long long>(a.coloring);
}

std::ostream& operator<<(std::ostream & os, ColoredGraph & cgraph) {
	os << "Graph: " << unsigned(cgraph.graph) << " Coloring: " << static_cast<long long unsigned>(cgraph.coloring)
		<< " Valid: " << cgraph.isValid() << " MinColors: " << cgraph.graph.minColors() << std::endl;

	unsigned col = cgraph.coloring.colors();
	for(unsigned i = 1; i <= col; i++) {
		std::queue<unsigned> Q;
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

//TODO: A better hashing function(too many collisions)
size_t std::hash<ColoredGraph>::operator()(const ColoredGraph & a) const {
	return static_cast<unsigned long long>(a.coloring) ^ static_cast<unsigned long long>(unsigned(a.graph));
}
