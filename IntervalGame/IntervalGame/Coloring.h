#pragma once
#include "Array4Bit.h"

class Coloring : public Array4Bit {
public:
	Coloring(unsigned long long a = 0);

	void reverse();
	unsigned normalize();
	bool isValid() const;
	unsigned colors() const;
	void insert(const unsigned position, const unsigned color);
};
