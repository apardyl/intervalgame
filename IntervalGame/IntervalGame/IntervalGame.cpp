// IntervalGame.cpp : Defines the entry point for the console application.
//

#include <iostream>

#include "ColoredGraph.h"

int main() {
	ColoredGraph c;
	c.insert(0, 5);
	c.insert(0, 1);
	c.insert(1, 6);
	c.insert(5, 1);

	std::cout << c;

    return 0;
}

