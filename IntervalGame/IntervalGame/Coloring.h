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
	class ConstProxy {
	private:
		const Coloring & coloring;
		unsigned key;
	public:
		ConstProxy(const Coloring & c, unsigned k);
		operator unsigned() const;
	};

	friend class Proxy;
	friend class ConstProxy;

	Proxy operator[](const unsigned k);
	ConstProxy operator[](const unsigned k) const;
	bool operator==(const Coloring& a) const;

	explicit operator unsigned long long() const;
	void reverse();
	unsigned normalize();
	bool isValid() const;
	unsigned colors() const;
	void insert(const unsigned position, const unsigned color);
};
