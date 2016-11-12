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

Graph::Proxy::operator bool const() const {
	return ((graph.val & (1 << key)) != 0);
}

Graph::Proxy Graph::operator[](const unsigned k) {
	if(k >= 32) throw std::out_of_range("No more than 32 elements allowed");
	return Proxy(*this, k);
}

bool Graph::operator==(const Graph & a) const {
	return val == a.val;
}

Graph::operator unsigned () const {
	return val;
}

unsigned Graph::popcount() const {
	return __builtin_popcount(val);
}

bool Graph::isValid() const {
	if (val == 0) return true;
	if((val & 1) == 0) return false;
	int count = 1;
	int open = 1;
	unsigned i = 2;
	for(;; i <<= 1) {
		count += ((val & i) == 0) ? -1 : (open++ , 1);
		if(count == 0) return open == popcount();
		if(i == (1 << 31)) return false;
	}
}

void Graph::reverse() {
	val = ~val & ((1 << (popcount() * 2)) - 1);
	unsigned tmp = 0;
	for(unsigned i = 1;; i <<= 1) {
		tmp <<= 1;
		tmp |= ((val & i) != 0) ? 1 : 0;
		if(i >= (unsigned(1) << (popcount() * 2 - 1))) break;
	}
	val = tmp;
}

void Graph::insert(const unsigned position, const bool flag) {
	if(position >= 32) throw std::out_of_range("No more than 32 elements allowed");
	if(val & (1<<31)) throw std::out_of_range("Graph full");

	//a work around for undefined behaviour when shifting left unsigned by its length
	unsigned mask = (position == 0) ? 0 : long long unsigned(~(unsigned(0))) >> (32 - position);

	val = ((val & (~mask)) << 1) | (val & mask) | (unsigned(flag) << position);
}

unsigned Graph::minColors() {
	unsigned used = 0;
	unsigned free = 0;
	for(unsigned i = 0, j = 0; i<32u && j<popcount(); i++) {
		if((*this)[i]) {
			j++;
			if (free <= 0) used++;
			else free--;
		}
		else {
			free++;
		}
	}
	return used;
}
