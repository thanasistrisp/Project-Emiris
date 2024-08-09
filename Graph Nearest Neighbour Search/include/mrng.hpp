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

		// Returns the set of neighbors of the node with the given index that lie at the minimum possible distance.
		void find_neighbors_with_min_distance(int, std::unordered_set<int>*);

		// Sets the navigating node for the graph that is used during search.
		void set_navigating_node();

	public:
		MRNG(const std::vector<std::vector<double>> &dataset);
		MRNG(const std::vector<std::vector<double>> &dataset, DirectedGraph *G);
		~MRNG();

		// Returns the indices of the k-approximate nearest neighbours (ANN) of the given query q
        // and their distances to the query based on the given distance function.
		// The search algorithm used is the Search-on-graph algorithm.
		std::tuple<std::vector<int>, std::vector<double>> query(const std::vector<double>&, unsigned int N, unsigned int L);

		static constexpr double (*distance)(const std::vector<double>&, const std::vector<double>&) = euclidean_distance;

		DirectedGraph *get_graph() const { return G; }
};