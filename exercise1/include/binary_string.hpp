#include <unordered_map>
#include <vector>

class binary_string
{
private:
	std::vector<int> p;
public:
	binary_string(std::vector<int> p);

	// declare equality operator
	bool operator==(const binary_string &other) const;

	// declare hash function
	struct hash {
		size_t operator()(const binary_string &bs) const;
	};
};