#include "Graph.h"
#include <stdexcept>

#ifdef _MSC_VER
#  include <intrin.h>
#  define __builtin_popcount __popcnt
#endif

Graph::Graph() : Graph(0) {
}

Graph::Graph(unsigned a) : val(a) {
}

Graph::Proxy::Proxy(Graph & c, unsigned k) : graph(c), key(k) {
}

void Graph::Proxy::operator=(const bool s) const {
	graph.val = (graph.val & ~(1 << key)) | (unsigned(s) << key);
}

Graph::Proxy::operator bool const&() const {
	return (graph.val & (1 << key)) != 0;
}

Graph::Proxy Graph::operator[](const unsigned k) {
	if(k >= 32) throw std::out_of_range("No more than 32 elements allowed");
	return Proxy(*this, k);
}

unsigned long long Graph::value() const {
	return val;
}

int Graph::size() const {
	return __builtin_popcount(val);
}

bool Graph::isValid() const {
	if((val & 1) == 0) return false;
	int count = 1;
	int open = 1;
	unsigned i = 2;
	for(;; i <<= 1) {
		count += ((val & i) == 0) ? -1 : (open++ , 1);
		if(count == 0) return open == size();
		if(i == (1 << 31)) return false;
	}
}

void Graph::reverse() {
	val = ~val & ((1 << (size() * 2)) - 1);
	unsigned tmp = 0;
	for(unsigned i = 1;; i <<= 1) {
		tmp <<= 1;
		tmp |= ((val & i) != 0) ? 1 : 0;
		if(i >= (1 << (size() * 2 - 1))) break;
	}
	val = tmp;
}