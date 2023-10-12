#pragma once

#include <vector>
#include <map>
#include <unordered_map>
#include "metrics.hpp"
#include "hash_function.h"

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
	std::vector<std::vector<int>> p_proj;

	std::vector<std::vector<int>> pack(std::vector<std::vector<int>> p_proj, int n, std::vector<int> q_proj, int hd);
	std::vector<std::vector<int>> preprocess(std::vector<std::vector<double>> p, int k);
	std::vector<HashFunction*> hash_functions;
	int f(int x, int i);
public:
	hypercube(std::vector<std::vector<double>> p, int k, int M, int probes, 
			  int N, double R, double (*distance)(std::vector<double>, std::vector<double> distance) = euclidean_distance);
	~hypercube();
	std::vector<int> query_n_nearest_neighbors(std::vector<double> q, std::vector<int> q_proj);
	std::vector<int> query_range(std::vector<double> q, std::vector<int> q_proj);
	std::vector<int> calculate_q_proj(std::vector<double> q);
	std::vector<std::vector<double>> get_dataset() { return p; }
	int get_N() { return N; }
	double (*distance)(std::vector<double>, std::vector<double> distance);
};