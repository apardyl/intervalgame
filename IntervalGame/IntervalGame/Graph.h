#pragma once
class Graph {
private:
	unsigned val;

public:
	Graph();
	Graph(unsigned);

	class Proxy {
	private:
		Graph & graph;
		unsigned key;
	public:
		Proxy(Graph & c, unsigned k);
		void operator=(const bool s) const;
		operator bool const&() const;
	};

	friend class Proxy;

	Proxy operator[](const unsigned k);
	operator unsigned const&() const;
};

