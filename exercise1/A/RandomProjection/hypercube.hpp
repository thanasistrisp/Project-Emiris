#pragma once

#include <vector>
#include <map>
#include <unordered_map>
#include "metrics.hpp"
#include "hash_function.h"
#include "binary_string.hpp"

class hypercube
{
private:
	std::vector<std::vector<double>> p;
	int k;
	int M;
	int probes;
	int N;
	double R;
	std::unordered_map<int, int> *f_map;

	// define HashTable
	typedef std::unordered_map<binary_string, std::vector<int>, binary_string::hash> HashTable;

	// std::vector<std::vector<int>> pack(const std::vector< std::vector<int>> &p_proj, int n, const std::vector<int> &q_proj, int hd);
	// std::vector<std::vector<int>> preprocess(const std::vector<std::vector<double>> &p, int k);
	std::vector<HashFunction*> hash_functions;
	int f(int x, int i);
public:
	hypercube(std::vector<std::vector<double>> p, int k, int M, int probes, 
			  int N, double R, double (*distance)(const std::vector<double> &, const std::vector<double> &) = euclidean_distance);
	// ~hypercube();
	// std::tuple<std::vector<int>, std::vector<double>> query_n_nearest_neighbors(const std::vector<double> &q, const std::vector<int> &q_proj);
	// std::vector<int> query_range(const std::vector<double> &q, const std::vector<int> &q_proj);
	std::vector<int> calculate_q_proj(const std::vector<double> &q);
	std::vector<std::vector<double>> get_dataset() { return p; }
	int get_N() { return N; }
	double (*distance)(const std::vector<double> &, const std::vector<double> &);
};