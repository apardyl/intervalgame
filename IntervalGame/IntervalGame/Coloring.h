#pragma once

class Coloring {
private:
	unsigned long long val;
public:
	Coloring();
	Coloring(unsigned long long);

	class Proxy {
	private:
		Coloring & coloring;
		unsigned key;
	public:
		Proxy(Coloring & c, int k);
		void operator=(const unsigned color) const;
		operator unsigned const&() const;
	};

	friend class Proxy;

	Proxy operator[](const unsigned k);
	operator unsigned long long const&() const;
};
