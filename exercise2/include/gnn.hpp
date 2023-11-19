#pragma once

#include <tuple>
#include <unordered_set>

#include "directed_graph.hpp"
#include "lp_metric.hpp"
#include "lsh.hpp"
#include "set_utils.hpp"

class GNN
{
	private:
		DirectedGraph *G;
		LSH *lsh;
		const std::vector<std::vector<double>> &dataset;
		int R;
		int E;

		void add_neighbors_pred(int, std::unordered_multiset<std::pair<int, double>*, decltype(&set_hash), decltype(&set_equal)>&, int);
		void add_neighbors_random(int, std::unordered_multiset<std::pair<int, double>*, decltype(&set_hash), decltype(&set_equal)>&, std::unordered_set<int>&, int);

	public:
		GNN(int k, const std::vector<std::vector<double>> &dataset, int R, int E);
		~GNN();
		std::tuple<std::vector<int>, std::vector<double>> query(const std::vector<double>&, unsigned int k,
																double (*distance)(const std::vector<double>&, const std::vector<double>&));
		static constexpr double (*distance)(const std::vector<double>&, const std::vector<double>&) = euclidean_distance;
};