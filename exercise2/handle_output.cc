#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <tuple>
#include <set>
// iterator is used for std::back_insert_iterator, std::advance().

#include "gnn.hpp"
#include "mrng.hpp"
#include "lp_metric.hpp"
#include "brute_force.hpp"

using namespace std;

#define distance euclidean_distance

// Writes the results of the queries to output file in the required format.
void handle_ouput(void *structure, const vector<vector<double>> &dataset, const vector<vector<double>> &queries, ofstream &output, vector<int> &params)
{
	// initialize parameters
	int E = params[0];
	int R = params[1];
	int l = params[2];
	int N = params[3];
	int m = params[4];
	if (m == 1) {
		cout << "Algorithm: GNN" << endl;
		output << "GNNS Results" << endl;
	}
	else {
		cout << "Algorithm: MRNG" << endl;
		output << "MRNG Results" << endl;
	}
	for (int q = 0; q < (int) queries.size(); q++) {
		cout << "Query: " << q << endl;
		output << "Query: " << q << endl;
		tuple<vector<int>, vector<double>> ann;
		clock_t start_ANN = clock();
		if (m == 1) {
			ann = ((GNN*) structure)->query(queries[q], N, E, R, distance);
		}
		else {
			ann = ((MRNG*) structure)->query(queries[q], N, l, distance);
		}
		clock_t end_ANN = clock();
		double elapsed_secs_ANN = double(end_ANN - start_ANN) / CLOCKS_PER_SEC;

		clock_t start_TNN = clock();
		tuple<vector<int>, vector<double>> tnn = brute_force(dataset, queries[q], N, distance);
		clock_t end_TNN = clock();
		double elapsed_secs_TNN = double(end_TNN - start_TNN) / CLOCKS_PER_SEC;
		
        vector<int> indices_ann = get<0>(ann);
        vector<double> distances_ann = get<1>(ann);
        vector<int> indices_tnn = get<0>(tnn);
        vector<double> distances_tnn = get<1>(tnn);
        for(int i = 0; (unsigned int) i < indices_ann.size(); i++){
			output << "Nearest neighbor-" << i+1 << ": " << indices_ann[i] << endl;
			output << "distanceApproximate: " << distances_ann[i] << endl;
			output << "distanceTrue: " << distances_tnn[i] << endl;
		}
		output << "tApproximate: " << elapsed_secs_ANN << endl;
		output << "tTrue: " << elapsed_secs_TNN << endl;
	}
	output.close();
}