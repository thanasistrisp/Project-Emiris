#pragma once

#include <tuple>
#include <unordered_set>

#include "directed_graph.hpp"

std::tuple<DirectedGraph*, std::unordered_set<int>> depth_first_search(const DirectedGraph &, int);