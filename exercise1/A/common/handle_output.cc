#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <fstream>

#include "hypercube.hpp"
#include "helper.hpp"

using namespace std;

void handle_ouput(string input_file, string query_file, string output_file, int k, int M, int probes, int N, double R, double (*distance)(vector<double>, vector<double>)) {
	vector<vector<double>> dataset = read_mnist_data(input_file);
	// get the first 1000 images to dataset
	dataset.resize(1000);
	vector<vector<double>> queries = read_mnist_data(query_file);
	queries.resize(100);

	ofstream output;
	output.open(output_file);

	hypercube cube(dataset, k, M, probes, N, R);

	for (int q = 0; q < (int) queries.size(); q++) {
		vector<int> q_proj = cube.calculate_q_proj(queries[q]);
		output << "Query: " << q << endl;
		vector<int> n_nearest_neighbors = cube.query_n_nearest_neighbors(queries[q], q_proj);
		for (int i = 0; i < N; i++) {
			output << "Nearest neighbor-" << i+1 << ": " << n_nearest_neighbors[i] << endl;
			output << "distanceHypercube: " << distance(dataset[n_nearest_neighbors[i]], queries[q]) << endl;
		}
		output << "R-near neighbors:" << endl;
		vector<int> range_neighbors = cube.query_range(queries[q], q_proj);
		for (int i = 0; i < (int) range_neighbors.size(); i++) {
			output << range_neighbors[i] << endl;
		}
	}

	output.close();
}