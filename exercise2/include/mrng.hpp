#pragma once

#include <tuple>

#include "directed_graph.hpp"
#include "lp_metric.hpp"
#include "lsh.hpp"

class MRNG
{
private:
	DirectedGraph *G;
	LSH *lsh;
	const std::vector<std::vector<double>> &dataset;
	int R;
	int E;

	void find_neighbors_with_min_distance(int, std::unordered_set<int>&);
public:
	MRNG(int k, const std::vector<std::vector<double>> &dataset, int R, int E);
	~MRNG();
	std::tuple<std::vector<int>, std::vector<double>> query(const std::vector<double>&, unsigned int k,
                                                            double (*distance)(const std::vector<double>&, const std::vector<double>&));
	static constexpr double (*distance)(const std::vector<double>&, const std::vector<double>&) = euclidean_distance;
};