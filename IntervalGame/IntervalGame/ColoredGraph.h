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
	bool isValid() const;
	void normalize();
	bool insert(unsigned position, unsigned end, unsigned color);
	unsigned popcount() const;
	unsigned colors();
	unsigned minColors();
	unsigned rawGraph() const;

	bool operator==(const ColoredGraph& a) const;
	bool operator<(const ColoredGraph& a) const;

	friend std::ostream& operator<<(std::ostream &, const ColoredGraph &);
	friend std::hash<ColoredGraph>;
};

namespace std {
	template <>
	struct hash<ColoredGraph> {
		size_t operator()(const ColoredGraph& a) const;
	};
}
