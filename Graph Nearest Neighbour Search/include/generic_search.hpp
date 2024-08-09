#pragma once

#include <vector>
#include <tuple>
#include <deque>

#include "directed_graph.hpp"

// Search-on-graph algorithm.

// Returns the indices of the k-approximate nearest neighbours (ANN) of the given query q
// and their distances to the query based on the given distance function.
// Parameters (in order): directed graph, dataset, start node, query, total candidates,
// number of nearest neighbors, distance function.
std::tuple<std::vector<int>, std::vector<double>> generic_search_on_graph(const DirectedGraph &, const std::vector<std::vector<double>>&,
                                                                          int, const std::vector<double>&, int, unsigned int,
                                                                          double (*distance)(const std::vector<double>&, const std::vector<double>&));

// Returns pairs of the indices and distances of the k-approximate nearest neighbours (ANN) of the given query q
// based on the given distance function.
// Parameters (in order): directed graph, dataset, start node, query, total candidates,
// number of nearest neighbors, distance function.
std::deque<std::pair<int, double>> generic_search_on_graph_checked(const DirectedGraph &, const std::vector<std::vector<double>>&,
                                                                   int, const std::vector<double>&, int,
                                                                   double (*distance)(const std::vector<double>&, const std::vector<double>&));