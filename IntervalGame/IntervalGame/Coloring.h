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
		Proxy(Coloring & c, unsigned k);
		void operator=(const unsigned color) const;
		operator unsigned() const;
	};

	friend class Proxy;

	Proxy operator[](const unsigned k);
	bool operator==(const Coloring& a) const;

	explicit operator unsigned long long() const;
	void reverse();
	unsigned normalize();
	bool isValid();
	unsigned colors();
	void insert(const unsigned position, const unsigned color);
};
