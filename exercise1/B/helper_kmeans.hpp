#pragma once

#include <vector>
#include <string>
#include <tuple>

#include "kmeans.hpp"

// Writes the results of the clustering to output file in the required format.
void handle_cluster_output(KMeans &kmeans, const std::string &output_file, bool complete, update_method method, const std::tuple<int,int,int,int,int,int> &config);