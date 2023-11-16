#pragma once

#include <tuple>

#include "directed_graph.hpp"
#include "lsh.hpp"

class GNN
{
private:
	DirectedGraph *G;
	LSH *lsh;
	const std::vector<std::vector<double>> &dataset;
	int R;
	int E;
public:
	GNN(int k, const std::vector<std::vector<double>> &dataset, int R, int E);
	~GNN();
	std::tuple<std::vector<int>, std::vector<double>> query(const std::vector<double>&, unsigned int k,
                                                            double (*distance)(const std::vector<double>&, const std::vector<double>&));
};