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
	int navigating_node;

	void find_neighbors_with_min_distance(int, std::unordered_set<int>*);
	void set_navigating_node();

public:
	MRNG(const std::vector<std::vector<double>> &dataset);
	~MRNG();

	std::tuple<std::vector<int>, std::vector<double>> query(const std::vector<double>&, unsigned int N, unsigned int L,
                                                            double (*distance)(const std::vector<double>&, const std::vector<double>&));
	static constexpr double (*distance)(const std::vector<double>&, const std::vector<double>&) = euclidean_distance;
};