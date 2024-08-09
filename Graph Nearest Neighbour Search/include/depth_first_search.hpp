#pragma once

#include <tuple>
#include <unordered_set>

#include "directed_graph.hpp"

// Performs DFS on the given graph, starting by the node with the given index
// and returns the DFS spanning tree of the graph and a set of nodes checked while searching.
std::tuple<DirectedGraph*, std::unordered_set<int>> depth_first_search(const DirectedGraph &, int);