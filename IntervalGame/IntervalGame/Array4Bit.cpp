#include "Array4Bit.h"
#include <stdexcept>

int Array4Bit::length() const {
	for(int i = 15; i >= 0; i--) {
		if((*this)[i] != 0) return i + 1;
	}
	return 0;
}

void Array4Bit::clear() {
	val[0] = 0;
	val[1] = 0;
}

size_t std::hash<Array4Bit>::operator()(const Array4Bit &a) const {
	return a.val[0] ^ a.val[1];
}

Array4Bit::Proxy Array4Bit::operator[](const unsigned k) {
	if(k >= 32) throw std::out_of_range("No more than 32 elements allowed");
	return Proxy(*this, k);
}

Array4Bit::ConstProxy Array4Bit::operator[](const unsigned k) const {
	if(k >= 32) throw std::out_of_range("No more than 32 elements allowed");
	return ConstProxy(*this, k);
}

bool Array4Bit::operator==(const Array4Bit &a) const {
	return val[0] == a.val[0] && val[1] == a.val[1];
}

bool Array4Bit::operator<(const Array4Bit &a) const {
	if(val[1] < a.val[1]) return true;
	if(val[1] == a.val[1] && val[0] < a.val[0]) return true;
	return false;
}

void Array4Bit::insert(int at, int v) {
	if(at >= 32) throw std::out_of_range("No more than 32 elements allowed");
	if((*this)[31] != 0) throw std::out_of_range("No more than 32 elements allowed");
	if(at >= 16) {
		insertIntoULL(val[1], at - 16, v);
	} else {
		val[1] <<= 4;
		(*this)[16] = (*this)[15];
		insertIntoULL(val[0], at, v);
	}
}

void Array4Bit::insertIntoULL(unsigned long long &into, int at, unsigned v) {
	unsigned long long mask = (at == 0) ? 0 : (~static_cast<unsigned long long>(0)) >> (64 - (at * 4));
	into = ((into & (~mask)) << 4) | (into & mask) | (static_cast<unsigned long long>(v) << at * 4);
}

Array4Bit::Proxy::Proxy(Array4Bit &c, unsigned k)
	: arr(c), key(k) {
}

Array4Bit::ConstProxy::ConstProxy(const Array4Bit &c, unsigned k)
	: arr(c), key(k) {
}

void Array4Bit::Proxy::operator=(const unsigned value) const {
	if(value >= 16) throw std::out_of_range("Values in range 0 to 15");
	if(key < 16) arr.val[0] = (arr.val[0] & ~(static_cast<unsigned long long>(0x0F) << 4 * key)) | (static_cast<unsigned long long>(value) << 4 * key);
	else arr.val[1] = (arr.val[1] & ~(static_cast<unsigned long long>(0x0F) << 4 * (key - 16))) | (static_cast<unsigned long long>(value) << 4 * (key - 16));
}

void Array4Bit::Proxy::operator=(const Proxy &p) const {
	*this = static_cast<unsigned>(p);
}

Array4Bit::Proxy::operator unsigned() const {
	if(key < 16) return ((arr.val[0] & (static_cast<unsigned long long>(0x0F) << 4 * key)) >> 4 * key);
	return ((arr.val[1] & (static_cast<unsigned long long>(0x0F) << 4 * (key - 16))) >> 4 * (key - 16));
}

Array4Bit::ConstProxy::operator unsigned() const {
	if(key < 16) return ((arr.val[0] & (static_cast<unsigned long long>(0x0F) << 4 * key)) >> 4 * key);
	return ((arr.val[1] & (static_cast<unsigned long long>(0x0F) << 4 * (key - 16))) >> 4 * (key - 16));
}

Array4Bit::Array4Bit()
	: val{0} {
}
