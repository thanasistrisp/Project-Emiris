#pragma once

#include <tuple>

#include "directed_graph.hpp"
#include "lp_metric.hpp"
#include "lsh.hpp"

class MRNG
{
private:
	const std::vector<std::vector<double>> &dataset;
	LSH *lsh = nullptr;
	DirectedGraph *G;
	int navigating_node;

	void find_neighbors_with_min_distance(int, std::unordered_set<int>*);
	void set_navigating_node();

public:
	MRNG(const std::vector<std::vector<double>> &dataset);
	MRNG(const std::vector<std::vector<double>> &dataset, DirectedGraph *G) : dataset(dataset), G(G) {}
	~MRNG();

	std::tuple<std::vector<int>, std::vector<double>> query(const std::vector<double>&, unsigned int N, unsigned int L);
	static constexpr double (*distance)(const std::vector<double>&, const std::vector<double>&) = euclidean_distance;
	DirectedGraph *get_graph() const { return G; }
};