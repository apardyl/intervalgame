#pragma once

#include "Graph.h"
#include "Coloring.h"
#include <ostream>

struct graphMapHash;

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
	unsigned colors() const;
	unsigned minColors() const;
	unsigned rawGraph() const;
	char shortColorHash() const;

	bool operator==(const ColoredGraph& a) const;
	bool operator<(const ColoredGraph& a) const;

	friend std::ostream& operator<<(std::ostream &, const ColoredGraph &);
	friend std::hash<ColoredGraph>;
	friend graphMapHash;
};

struct graphMapHash {
	size_t operator()(const ColoredGraph& a) const;
};

namespace std {
	template <>
	struct hash<ColoredGraph> {
		size_t operator()(const ColoredGraph& a) const;
	};
}
