#include <iostream>

#include "hash_table.h"

using namespace std;

#include <vector>

// class d
// {
// 	private:
// 		int x;

// 	public:
// 		d(int);
// 		~d();
// };

// d::d(int x)
// : x(x)
// {

// }

// d::~d()
// {

// }

int primary(const std::vector<int>& v)
{
	return v[0];
}

unsigned int secondary(const std::vector<int>& v)
{
	return v[1];
}

int main(void) {
	cout << "LSH test" << endl;

	// d dd(2);

	std::vector<int> v{1, 2, 3};

	HashTable<std::vector<int>> hash_table(10, primary, secondary);
	hash_table.insert(&v);
}