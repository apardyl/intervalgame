#include "Coloring.h"
#include <stdexcept>


Coloring::Coloring(unsigned long long value) : val(value) {
}

Coloring::Coloring() : Coloring(0) {
}

void Coloring::reverse() {
	int i = 15;
	for (; i >= 0; i--) {
		if ((*this)[i] != 0) break;
	}
	unsigned tmp;
	for(int j = 0; j<i;j++, i--) {
		tmp = (*this)[i];
		(*this)[i] = unsigned((*this)[j]);
		(*this)[j] = tmp;
	}
}

unsigned Coloring::normalize() {
	unsigned perm[16] = { 0 };
	unsigned next = 1;
	for(int i = 0; i<16;i++) {
		if ((*this)[i] == 0) break;
		if (perm[(*this)[i]] == 0) {
			perm[(*this)[i]] = next;
			next++;
		}
		(*this)[i] = perm[(*this)[i]];
	}
	return next - 1;
}

bool Coloring::isValid() {
	int i = 0;
	for(;i<16;i++) {
		if ((*this)[i] == 0) break;
	}
	for(;i<16;i++) {
		if ((*this)[i] != 0) return false;
	}
	return true;
}

Coloring::Proxy Coloring::operator[](const unsigned k) {
	if(k >= 16) throw std::out_of_range("No more than 16 elements allowed");
	return Proxy(*this, k);
}

unsigned long long Coloring::value() const {
	return val;
}

Coloring::Proxy::Proxy(Coloring & c, unsigned k) : coloring(c), key(k) {
}

void Coloring::Proxy::operator=(const unsigned color) const {
	if(color >= 16) throw std::out_of_range("Colors in range 0 to 15");
	coloring.val = (coloring.val & ~(0x0F << 4 * key)) | (unsigned long long(color) << 4 * key);
}

Coloring::Proxy::operator unsigned const() const {
	return ((coloring.val & (0x0F << 4 * key)) >> 4 * key);
}
