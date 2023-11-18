#pragma once

#include <tuple>

#include "directed_graph.hpp"
#include "lsh.hpp"

class MRNG
{
private:
	DirectedGraph *G;
	LSH *lsh;
	const std::vector<std::vector<double>> &dataset;
	int navigating_node;
	int R;
	int E;

	void set_navigating_node();

public:
	MRNG(int k, const std::vector<std::vector<double>> &dataset, int R, int E);
	~MRNG();

	std::tuple<std::vector<int>, std::vector<double>> query(const std::vector<double>&, unsigned int k,
                                                            double (*distance)(const std::vector<double>&, const std::vector<double>&));
};