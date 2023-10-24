#pragma once

#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include "lp_metric.h"
#include "hash_function.h"
#include "binary_string.hpp"

class hypercube
{
private:
	const std::vector<std::vector<double>> &p;
	int k;
	int M;
	int probes;

	std::unordered_map<int, int> *f_map;

	std::unordered_set<binary_string, binary_string::hash> *used_vertices;

	// define HashTable
	typedef std::unordered_map<binary_string, std::vector<int>, binary_string::hash> HashTable;
	HashTable *hash_table;

	std::vector<HashFunction*> hash_functions;
	int f(int x, int i);
	std::vector<std::vector<int>> pack(const std::vector<int> &q_proj, int hamming_distance);
	std::set<std::vector<int>> get_permutation(const std::vector<int> &q_proj, int hamming_distance);
public:
	hypercube(const std::vector<std::vector<double>> &p, int k, int M, int probes, 
			  double (*distance)(const std::vector<double> &, const std::vector<double> &) = euclidean_distance);
	~hypercube();
	std::tuple<std::vector<int>, std::vector<double>> query_n_nearest_neighbors(const std::vector<double> &q, const std::vector<int> &q_proj, int N);
	std::tuple<std::vector<int>, std::vector<double>> query_range(const std::vector<double> &q, const std::vector<int> &q_proj, double R);
	std::vector<int> calculate_q_proj(const std::vector<double> &q);
	std::vector<std::vector<double>> get_dataset() { return p; }
	double (*distance)(const std::vector<double> &, const std::vector<double> &);
};