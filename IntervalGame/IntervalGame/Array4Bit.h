#pragma once
class Array4Bit {
protected:
	unsigned long long val;
public:
	Array4Bit(unsigned long long a = 0);

	class Proxy {
	private:
		Array4Bit & arr;
		unsigned key;
	public:
		Proxy(Array4Bit & c, unsigned k);
		void operator=(const unsigned value) const;
		operator unsigned() const;
	};
	class ConstProxy {
	private:
		const Array4Bit & arr;
		unsigned key;
	public:
		ConstProxy(const Array4Bit & c, unsigned k);
		operator unsigned() const;
	};

	friend class Proxy;
	friend class ConstProxy;

	Proxy operator[](const unsigned k);
	ConstProxy operator[](const unsigned k) const;
	bool operator==(const Array4Bit& a) const;

	explicit operator unsigned long long() const;
	void reverse();
};

