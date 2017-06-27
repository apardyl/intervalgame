#pragma once

#include <functional>

class Array4Bit {
private:
	static void insertIntoULL(unsigned long long int &into, int at, unsigned v);
protected:
	unsigned long long val[2];
public:
	class Proxy {
	private:
		Array4Bit &arr;
		unsigned key;
	public:
		Proxy(Array4Bit &c, unsigned k);
		void operator=(const unsigned value) const;
		void operator=(const Proxy &p) const;
		operator unsigned() const;
	};

	class ConstProxy {
	private:
		const Array4Bit &arr;
		unsigned key;
	public:
		ConstProxy(const Array4Bit &c, unsigned k);
		operator unsigned() const;
	};

	friend class Proxy;
	friend class ConstProxy;

	Array4Bit();

	Proxy operator[](const unsigned k);
	ConstProxy operator[](const unsigned k) const;
	bool operator==(const Array4Bit &a) const;
	bool operator<(const Array4Bit &a) const;
	void insert(int at, int v);

	int length() const;
	void clear();

	friend std::hash<Array4Bit>;
};

namespace std {
	template <>
	struct hash<Array4Bit> {
		size_t operator()(const Array4Bit &a) const;
	};
}
