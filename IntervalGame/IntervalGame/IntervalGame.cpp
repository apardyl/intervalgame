// IntervalGame.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <unordered_map>
#include <map>
#include <algorithm>

#include "ColoredGraph.h"

unsigned depth = 0;

std::map<ColoredGraph, double> gameStates;

double minMax(ColoredGraph cgraph) {
	unsigned colors = cgraph.colors();

	if(cgraph.popcount() >= depth) {
		return double(colors) / cgraph.minColors();
	}

	double max = -DBL_MAX;

	for(unsigned i = 0; i <= cgraph.popcount() * 2;i++) {
		double min = DBL_MAX;
		for(unsigned j = 1; j <= colors + 1; j++) {
			ColoredGraph ncgraph = cgraph;
			ncgraph.insert(i, j);

			if (!ncgraph.isValid()) continue;
			ncgraph.normailze();

			double x;
			if (gameStates.count(cgraph) == 1) x = gameStates[cgraph];
			else x = minMax(ncgraph);
			
			min = std::min(min, x);
		}
		if(min != DBL_MAX) max = std::max(max, min);
	}

	gameStates[cgraph] = max;
	return max;
}

int main() {

	for (int i = 1; i < 16;i++) {
		gameStates.clear();
		depth = i;
		ColoredGraph graph;
		double x = minMax(graph);
		std::cout << i << ": k: " << x  << " mapSize: " << gameStates.size() << std::endl;
	}

	std::cin.get();

    return 0;
}

