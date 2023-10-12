#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <fstream>
#include <limits>
#include <algorithm>

#include "hypercube.hpp"
#include "helper.hpp"

using namespace std;

// brute force search for nearest neighbors
static vector<int> brute_force(vector<vector<double>> dataset, vector<double> query, int N, double (*distance)(vector<double>, vector<double>)) {
	vector<int> nearest_neighbors;
	vector<double> distances;
	for (int i = 0; i < (int) dataset.size(); i++) {
		distances.push_back(distance(dataset[i], query));
	}
	for (int i = 0; i < N; i++) {
		int min_index = min_element(distances.begin(), distances.end()) - distances.begin();
		nearest_neighbors.push_back(min_index);
		distances[min_index] = numeric_limits<double>::max();
	}
	return nearest_neighbors;
}

void handle_ouput(hypercube &cube, ofstream &output, const vector<vector<double>> &queries) {
	for (int q = 0; q < (int) queries.size(); q++) {
		vector<int> q_proj = cube.calculate_q_proj(queries[q]);
		cout << "Query: " << q << endl;
		output << "Query: " << q << endl;
		clock_t start_ANN = clock();
		vector<int> n_nearest_neighbors = cube.query_n_nearest_neighbors(queries[q], q_proj);
		clock_t end_ANN = clock();
		double elapsed_secs_ANN = double(end_ANN - start_ANN) / CLOCKS_PER_SEC;

		clock_t start_ENN = clock();
		vector<int> real_nearest_neighbors = brute_force(cube.get_dataset(), queries[q], cube.get_N(), cube.distance);
		clock_t end_ENN = clock();
		double elapsed_secs_ENN = double(end_ENN - start_ENN) / CLOCKS_PER_SEC;
		
		for (int i = 0; i < cube.get_N(); i++) {
			output << "Nearest neighbor-" << i+1 << ": " << n_nearest_neighbors[i] << endl;
			output << "distanceHypercube: " << cube.distance(cube.get_dataset()[n_nearest_neighbors[i]], queries[q]) << endl;
			output << "distanceTrue: " << cube.distance(cube.get_dataset()[real_nearest_neighbors[i]], queries[q]) << endl;
		}
		output << "tHypercube: " << elapsed_secs_ANN << endl;
		output << "tTrue: " << elapsed_secs_ENN << endl;
		output << "R-near neighbors:" << endl;
		vector<int> range_neighbors = cube.query_range(queries[q], q_proj);
		for (int i = 0; i < (int) range_neighbors.size(); i++) {
			output << range_neighbors[i] << endl;
		}
	}

	output.close();
}