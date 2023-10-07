#include <iostream>
#include <random>
#include <vector>
#include "defines.h"

using namespace std;

// h takes as input a vector of dimension d and returns an integer
int h(vector<double> p) {
	vector<double> v(p.size());
	// set seed as cpu clock
	int seed = clock();
	default_random_engine generator(seed);
	normal_distribution<double> distribution(0.0, 1.0);
	for (int i = 0; i < (int) p.size(); i++) {
		v[i] = distribution(generator);
	}

	// compute dot product of p and v
	double dot_product = 0;
	for (int i = 0; i < (int) p.size(); i++) {
		dot_product += p[i] * v[i];
	}
	dot_product = abs(dot_product); // normalize

	// select random t from [0, w)
	default_random_engine generator2(seed);
	uniform_int_distribution<int> distribution2(0, w-1);
	int t = distribution2(generator2);

	// compute h(p) = floor((dot_product + t) / w)
	return (int) floor((dot_product + t) / w);
}