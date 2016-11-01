#include "Coloring.h"
#include <exception>
#include <stdexcept>

Coloring::Coloring(unsigned long long value) : val(value) {
}

Coloring::Coloring() : Coloring(0) {
}

Coloring::operator unsigned long long const&() const {
	return val;
}

Coloring::Proxy Coloring::operator[](const unsigned k) {
	if(k >= 16) throw std::out_of_range("No more than 16 elements allowed");
	return Proxy(*this, k);
}

Coloring::Proxy::Proxy(Coloring & c, int k) : coloring(c), key(k) {
}

void Coloring::Proxy::operator=(const unsigned color) const {
	if(color >= 16) throw std::domain_error("Colors in range 0 to 15");
	coloring.val = (coloring.val & ~(0x0F << 4 * key)) | (unsigned long long(color) << 4 * key);
}

Coloring::Proxy::operator unsigned const&() const {
	return unsigned((coloring.val & (0x0F << 4 * key)) >> 4 * key);
}
