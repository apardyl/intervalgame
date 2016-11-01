#include "Graph.h"
#include <stdexcept>

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
	return graph.val & (1 << key) != 0;
}

Graph::Proxy Graph::operator[](const unsigned k) {
	if (k >= 32) throw std::out_of_range("No more than 32 elements allowed");
	return Proxy(*this, k);
}

Graph::operator unsigned const&() const {
	return val;
}
