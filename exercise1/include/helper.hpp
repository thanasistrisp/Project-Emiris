#pragma once

#include <vector>
#include <string>

std::vector<std::vector<double>> read_mnist_data(std::string filename);
void export_image(std::vector<double> image, std::string filename);
void handle_ouput(std::string input_file, std::string query_file, std::string output_file, int k, int M, int probes, int N, double R, double (*distance)(std::vector<double>, std::vector<double>) = euclidean_distance);