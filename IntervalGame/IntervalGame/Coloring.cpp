#include "Coloring.h"
#include <stdexcept>


Coloring::Coloring(unsigned long long value) : val(value) {
}

Coloring::Coloring() : Coloring(0) {
}

void Coloring::reverse() {
	int i = 15;
	for(; i >= 0; i--) {
		if((*this)[i] != 0) break;
	}
	unsigned tmp;
	for(int j = 0; j < i; j++ , i--) {
		tmp = (*this)[i];
		(*this)[i] = unsigned((*this)[j]);
		(*this)[j] = tmp;
	}
}

unsigned Coloring::normalize() {
	unsigned perm[16] = {0};
	unsigned next = 1;
	for(int i = 0; i < 16; i++) {
		if((*this)[i] == 0) break;
		if(perm[(*this)[i]] == 0) {
			perm[(*this)[i]] = next;
			next++;
		}
		(*this)[i] = perm[(*this)[i]];
	}
	return next - 1;
}

bool Coloring::isValid() const {
	int i = 0;
	for(; i < 16; i++) {
		if((*this)[i] == 0) break;
	}
	for(; i < 16; i++) {
		if((*this)[i] != 0) return false;
	}
	return true;
}

unsigned Coloring::colors() const {
	unsigned perm[16] = {0};
	unsigned next = 1;
	for(int i = 0; i < 16; i++) {
		if((*this)[i] == 0) break;
		if(perm[(*this)[i]] == 0) {
			perm[(*this)[i]] = next;
			next++;
		}
	}
	return next - 1;
}

void Coloring::insert(const unsigned position, const unsigned color) {
	if(position >= 16) throw std::out_of_range("No more than 16 elements allowed");
	if(color >= 16) throw std::out_of_range("Colors in range 0 to 15");
	if((val & (static_cast<unsigned long long>(0x0F) << (4 * 15))) != 0) throw std::out_of_range("Coloring full");

	//a work around for undefined behaviour when shifting left unsigned by its length
	unsigned long long mask = (position == 0) ? 0 : (~static_cast<unsigned long long>(0)) >> (64 - (position * 4)); 

	val = ((val & (~mask)) << 4) | (val & mask) | (static_cast<unsigned long long>(color) << position * 4);
}

char Coloring::shortHash() const {
	char h = 0;
	for(unsigned long long v = val; v != 0; v>>=2) {
		h ^= v & 0x3;
	}
	return h;
}

Coloring::operator unsigned long long() const {
	return val;
}

Coloring::Proxy Coloring::operator[](const unsigned k) {
	if(k >= 16) throw std::out_of_range("No more than 16 elements allowed");
	return Proxy(*this, k);
}

Coloring::ConstProxy Coloring::operator[](const unsigned k) const {
	if(k >= 16) throw std::out_of_range("No more than 16 elements allowed");
	return ConstProxy(*this, k);
}

bool Coloring::operator==(const Coloring & a) const {
	return val == a.val;
}

Coloring::Proxy::Proxy(Coloring & c, unsigned k) : coloring(c), key(k) {
}

Coloring::ConstProxy::ConstProxy(const Coloring & c, unsigned k) : coloring(c), key(k) {
}

void Coloring::Proxy::operator=(const unsigned color) const {
	if(color >= 16) throw std::out_of_range("Colors in range 0 to 15");
	coloring.val = (coloring.val & ~(static_cast<unsigned long long>(0x0F) << 4 * key)) | (static_cast<unsigned long long>(color) << 4 * key);
}

Coloring::Proxy::operator unsigned() const {
	return ((coloring.val & (static_cast<unsigned long long>(0x0F) << 4 * key)) >> 4 * key);
}

Coloring::ConstProxy::operator unsigned() const {
	return ((coloring.val & (static_cast<unsigned long long>(0x0F) << 4 * key)) >> 4 * key);
}
