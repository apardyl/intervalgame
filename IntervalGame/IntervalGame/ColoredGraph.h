#pragma once

#include "Graph.h"
#include "Coloring.h"
#include <ostream>

class ColoredGraph {
private:
	Graph graph;
	Coloring coloring;
public:
	ColoredGraph();
	bool isValid();
	void normailze();
	void insert(unsigned position, unsigned color);

	bool operator==(const ColoredGraph& a) const;

	friend std::ostream& operator<<(std::ostream &, ColoredGraph &);
	friend std::hash<ColoredGraph>;
};

namespace std {
	template <>
	struct hash<ColoredGraph> {
		size_t operator()(const ColoredGraph& a) const;
	};
}