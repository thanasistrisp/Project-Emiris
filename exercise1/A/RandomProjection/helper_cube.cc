#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <algorithm>
#include <map>
#include <random>
#include <tuple>
#include <set>

#include "lp_metric.hpp"
#include "hypercube.hpp"

using namespace std;

std::vector<std::vector<int>> hypercube::pack(const std::vector<int> &q_proj, int hamming_distance)
{
	std::vector<std::vector<int>> result;
	for (auto it = hash_table->begin(); it != hash_table->end(); it++) { // for every bucket
		if (it->first.hamming_distance(q_proj, hamming_distance)) {
			// push every vertex in the bucket to result
			result.push_back(it->second);
			used_vertices->erase(it->first);
		}
	}
	return result;
}

int hypercube::f(int x, int i) {
	// If f_map[i] does not contain x, calculate f_i(x) and store it in f_map[i] else return the stored value.
	if (f_map[i].find(x) == f_map[i].end()) {
		f_map[i][x] = rand() % 2;
	}
	return f_map[i][x];
}