#pragma once

#include <vector>
#include <string>
#include <tuple>

// Reads the dataset from the given file and returns a vector of vectors (Important: it does not check if the file exists).
std::vector<std::vector<double>> read_mnist_data(const std::string &filename, int num=0);
std::vector<std::vector<double>> read_mnist_data_float(const std::string &filename, int num=0);
std::vector<double> get_mnist_float_index(const std::string &filename, int index);

// Reads the config file and returns a tuple of the parameters (Important: it does not check if the file exists).
/* The tuple contains:
 * 1. k of the Kmeans (number of clusters)
 * 2. number L of the LSH (number of hash tables)
 * 3. k of the LSH (number of hash functions)
 * 4. M of the Hypercube (number of candidates)
 * 5. k of the Hypercube (number of dimensions)
 * 6. number of probes of the Hypercube
 */
std::tuple<int, int, int, int, int, int> read_config_file(const std::string &filename);

void export_image(const std::vector<double> &image, std::string filename);

bool file_exists(const std::string &filename);