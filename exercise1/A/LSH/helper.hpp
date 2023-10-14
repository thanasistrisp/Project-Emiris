#pragma once

#include <vector>
#include <string>

#include "lsh.h"

std::vector<std::vector<double>> read_mnist_data(std::string filename); // does not check if file exists
void export_image(std::vector<double> image, std::string filename);
void handle_ouput(LSH &cube, const std::vector<std::vector<double>> &dataset,
                  const std::vector<std::vector<double>> &queries, int n, double r, std::ofstream &output);
bool file_exists(std::string filename);