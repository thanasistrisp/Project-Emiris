#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <fstream>
#include <limits>
#include <algorithm>

#include "hypercube.hpp"
#include "helper.hpp"
#include "brute_force.hpp"

using namespace std;

// Writes the results of the queries to output file in the required format.
void handle_ouput(hypercube &cube, ofstream &output, const vector<vector<double>> &queries, double R, int N)
{
	for (int q = 0; q < (int) queries.size(); q++) {
		vector<vector<double>> dataset = cube.get_dataset();
		vector<int> q_proj = cube.calculate_q_proj(queries[q]);
		cout << "Query: " << q << endl;
		output << "Query: " << q << endl;
		clock_t start_ANN = clock();
		vector<int> n_nearest_neighbors;
		vector<double> dist_ann;
		tie(n_nearest_neighbors, dist_ann) = cube.query_n_nearest_neighbors(queries[q], q_proj, N);
		clock_t end_ANN = clock();
		double elapsed_secs_ANN = double(end_ANN - start_ANN) / CLOCKS_PER_SEC;

		clock_t start_ENN = clock();
		vector<int> real_nearest_neighbors;
		vector<double> dist_true;
		tie(real_nearest_neighbors, dist_true) = brute_force(dataset, queries[q], N, cube.distance);
		clock_t end_ENN = clock();
		double elapsed_secs_ENN = double(end_ENN - start_ENN) / CLOCKS_PER_SEC;
		
		for (int i = 0; i < N; i++) {
			output << "Nearest neighbor-" << i+1 << ": " << n_nearest_neighbors[i] << endl;
			output << "distanceHypercube: " << cube.distance(dataset[n_nearest_neighbors[i]], queries[q]) << endl;
			output << "distanceTrue: " << dist_true[i] << endl;
		}
		output << "tHypercube: " << elapsed_secs_ANN << endl;
		output << "tTrue: " << elapsed_secs_ENN << endl;
		output << "R-near neighbors:" << endl;
		vector<int> rn_indices;	
		vector<double> rn_distances;
		tie(rn_indices, rn_distances) = cube.query_range(queries[q], q_proj, R);
		for (int i = 0; i < (int) rn_indices.size(); i++) {
			output << rn_indices[i] << endl;
		}
		output << endl;
	}

	output.close();
}