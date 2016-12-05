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
		void operator=(const bool s);
		operator bool() const;
	};
	class ConstProxy {
	private:
	  const Graph & graph;
	  unsigned key;
	public:
	  ConstProxy(const Graph & c, unsigned k);
	  operator bool() const;
  };

	friend class Proxy;
	friend class ConstProxy;

	Proxy operator[](const unsigned k);
	ConstProxy operator[](const unsigned k) const;
	bool operator==(const Graph&) const;

	explicit operator unsigned() const;
	unsigned popcount() const;
	bool isValid() const;
	void reverse();
	void insert(const unsigned position, const bool flag);
	unsigned minColors() const;
};
