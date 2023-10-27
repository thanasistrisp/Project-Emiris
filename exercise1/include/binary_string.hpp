#pragma once

#include <unordered_map>
#include <vector>

// Used with unordered_map.
class binary_string
{
private:
	std::vector<int> p;
public:
	binary_string(std::vector<int> p);

	bool operator==(const binary_string &other) const;

	// Hash function that returns the decimal value of the binary string.
	struct hash {
		size_t operator()(const binary_string &bs) const;
	};
};