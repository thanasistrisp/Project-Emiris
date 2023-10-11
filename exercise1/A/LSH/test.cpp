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

	HashTable<std::vector<double>, int> a(10, 2, 3, 4);

	std::vector<double> v1{2.0, 3.0};
	std::vector<double> v2{1.0, 4.0};
	std::vector<double> v3{0.0, 2.0};

	a.insert(v1, 0);
	a.insert(v2, 1);
	a.insert(v3, 2);
}