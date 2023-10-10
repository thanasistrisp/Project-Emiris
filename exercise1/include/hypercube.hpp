#pragma once

#include <vector>
#include <map>
#include "metrics.hpp"

class hypercube
{
private:
	std::vector<std::vector<double>> p;
	int k;
	int M;
	int probes;
	int N;
	double R;
	double (*distance)(std::vector<double>, std::vector<double> distance);
	std::vector<std::vector<int>> p_proj;
public:
	hypercube(std::vector<std::vector<double>> p, int k, int M, int probes, 
			  int N, double R, double (*distance)(std::vector<double>, std::vector<double> distance) = euclidean_distance);
	std::vector<std::vector<double>> query_n_nearest_neighbors(std::vector<double> q);
	std::vector<std::vector<double>> query_range(std::vector<double> q);
	std::vector<std::vector<int>> pack(std::vector<std::vector<int>> p_proj, int n, std::vector<int> q_proj, int hd);
	std::vector<int> calculate_q_proj(std::vector<double> q);
};


std::vector<std::vector<int>> pack(std::vector<std::vector<int>> p_proj, int n, std::vector<int> q_proj);
std::vector<std::vector<int>> preprocess(std::vector<std::vector<double>> p, int k);
int f(int x);