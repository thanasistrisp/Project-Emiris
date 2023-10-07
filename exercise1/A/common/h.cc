#include <iostream>
#include <random>
#include <vector>
#include "defines.h"

using namespace std;

// h takes as input a vector of dimension d and returns an integer
int h(vector<double> p) {
	vector<double> v(p.size());
	default_random_engine generator;
	normal_distribution<double> distribution(0.0, 1.0);
	for (int i = 0; i < (int) p.size(); i++) {
		v[i] = distribution(generator);
	}

	// compute dot product of p and v
	double dot_product = 0;
	for (int i = 0; i < (int) p.size(); i++) {
		dot_product += p[i] * v[i];
	}

	// select random t from [0, w]
	default_random_engine generator2;
	uniform_int_distribution<int> distribution2(0, w);
	int t = distribution2(generator2);

	// compute h(p) = floor((dot_product + t) / w)
	return (int) floor((dot_product + t) / w);
}