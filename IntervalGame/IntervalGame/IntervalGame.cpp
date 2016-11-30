// IntervalGame.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <unordered_map>
#include <map>
#include <algorithm>
#include <float.h>

#include "ColoredGraph.h"

unsigned depth = 0;
unsigned target_width = 0;

std::unordered_map<ColoredGraph, double> gameStates;

double score(ColoredGraph cgraph) {
  if ( target_width > 0 ) {
    if ( cgraph.colors() == target_width )
    	return double(cgraph.colors()) / cgraph.minColors();
    return 1;
  }
  return double(cgraph.colors()) / cgraph.minColors();
}

double minMax(ColoredGraph cgraph) {
	if (gameStates.count(cgraph) == 1) return gameStates[cgraph];

	unsigned colors = cgraph.colors();

	if(cgraph.popcount() >= depth) {
	  return score(cgraph);
	}

	double max = -DBL_MAX;

	for(unsigned i = 0; i <= cgraph.popcount() * 2 + 1;i++) {
		bool stop = false;
		for (unsigned k = 0; k <= cgraph.popcount() && !stop; k++) {
			double min = DBL_MAX;
			for (unsigned j = 1; j <= colors + 1; j++) {
				ColoredGraph ncgraph = cgraph;
				if (!ncgraph.insert(i, k, j)) {
					stop = true;
					break;
				}

				if (!ncgraph.isValid()) continue;
				ncgraph.normalize();

				double x;
				if (gameStates.count(ncgraph) == 1) x = gameStates[ncgraph];
				else x = minMax(ncgraph);

				min = std::min(min, x);
			}
			if (min != DBL_MAX) max = std::max(max, min);
		}
	}

  max = std::max(max, score(cgraph));

	gameStates[cgraph] = max;
	//std::cout << cgraph << max << std::endl;
	return max;
}

int main() {

	for (int i = 1; i <=16;i++) {
		gameStates.clear();
		depth = i;
		ColoredGraph graph;
		double x = minMax(graph);
		std::cout << i << ": k: " << x  << " mapSize: " << gameStates.size() << std::endl;
	}

	std::cin.get();

    return 0;
}
