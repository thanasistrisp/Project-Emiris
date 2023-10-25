#include <vector>
#include <cmath>
#include "binary_string.hpp"

using namespace std;

binary_string::binary_string(vector<int> p) {
	this->p = p;
}

bool binary_string::operator==(const binary_string &other) const {
	return p == other.p;
}

size_t binary_string::hash::operator()(const binary_string &bs) const {
	size_t hash = 0;
	for (int i = 0; i < (int) bs.p.size(); i++) {
		hash += bs.p[i] * pow(2, i);
	}
	return hash;
}