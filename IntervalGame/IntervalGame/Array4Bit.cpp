#include "Array4Bit.h"
#include <stdexcept>


Array4Bit::Array4Bit(unsigned long long a)
	: val(a) {
}

Array4Bit::operator unsigned long long() const {
	return val;
}

Array4Bit::Proxy Array4Bit::operator[](const unsigned k) {
	if(k >= 16) throw std::out_of_range("No more than 16 elements allowed");
	return Proxy(*this, k);
}

Array4Bit::ConstProxy Array4Bit::operator[](const unsigned k) const {
	if(k >= 16) throw std::out_of_range("No more than 16 elements allowed");
	return ConstProxy(*this, k);
}

bool Array4Bit::operator==(const Array4Bit &a) const {
	return val == a.val;
}

Array4Bit::Proxy::Proxy(Array4Bit &c, unsigned k) : arr(c), key(k) {
}

Array4Bit::ConstProxy::ConstProxy(const Array4Bit &c, unsigned k) : arr(c), key(k) {
}

void Array4Bit::Proxy::operator=(const unsigned color) const {
	if(color >= 16) throw std::out_of_range("Colors in range 0 to 15");
	arr.val = (arr.val & ~(static_cast<unsigned long long>(0x0F) << 4 * key)) | (static_cast<unsigned long long>(color) << 4 * key);
}

Array4Bit::Proxy::operator unsigned() const {
	return ((arr.val & (static_cast<unsigned long long>(0x0F) << 4 * key)) >> 4 * key);
}

Array4Bit::ConstProxy::operator unsigned() const {
	return ((arr.val & (static_cast<unsigned long long>(0x0F) << 4 * key)) >> 4 * key);
}
