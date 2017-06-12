#pragma once
#include "Array4Bit.h"
#include <ostream>

class IntervalGraph {
private:
	Array4Bit graph;
	int normailzeColors();
	void reverse();
public:
	bool isValid(int maxNested) const;
	void normalize();
	bool insert(unsigned start, unsigned length, unsigned color);
	unsigned length() const;
	unsigned colors() const;
	unsigned minColors() const;

	bool operator==(const IntervalGraph& a) const;
	bool operator<(const IntervalGraph& a) const;

	friend std::ostream& operator<<(std::ostream &, const IntervalGraph &);
	friend std::hash<IntervalGraph>;
};

namespace std {
	template <>
	struct hash<IntervalGraph> {
		size_t operator()(const IntervalGraph& a) const;
	};
}
