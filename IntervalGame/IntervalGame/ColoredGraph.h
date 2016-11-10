#pragma once

#include "Graph.h"
#include "Coloring.h"
#include <ostream>

class ColoredGraph {
public: //TODO: change to private
	Graph graph;
	Coloring coloring;
public:
	ColoredGraph();
	bool isValid();
	void normailze();

	friend std::ostream& operator<<(std::ostream&, ColoredGraph&);
};
