#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <tuple>
#include <set>
#include <limits>
// iterator is used for std::back_insert_iterator, std::advance().

#include "approximate_knn_graph.hpp"
#include "mrng.hpp"
#include "nsg.hpp"
#include "lp_metric.hpp"
#include "brute_force.hpp"

using namespace std;

void handle_ouput(void *structure, const vector<vector<double>> &dataset, const vector<vector<double>> &queries, ofstream &output, vector<int> &params)
{
	// Initialize parameters.
	int E = params[0];
	int R = params[1];
	int l = params[2];
	int N = params[3];
	int lq = params[4];
	int m = params[5];
	if (m == 1) {
		cout << "Algorithm: GNN" << endl;
		output << "GNNS Results" << endl;
	}
	else if (m == 2) {
		cout << "Algorithm: MRNG" << endl;
		output << "MRNG Results" << endl;
	}
	else {
		cout << "Algorithm: NSG" << endl;
		output << "NSG Results" << endl;
	}

	double elapsed_secs_ANN = 0;
	double elapsed_secs_TNN = 0;
	double aaf = 0; // Average approximate factor.

	for (int q = 0; q < (int) queries.size(); q++) {
		cout << "Query: " << q << endl;
		output << "Query: " << q << endl;
		tuple<vector<int>, vector<double>> ann;
		clock_t start_ANN = clock();
		if (m == 1) {
			ann = ((ApproximateKNNGraph*) structure)->query(queries[q], N, E, R);
		}
		else if (m == 2) {
			ann = ((MRNG*) structure)->query(queries[q], N, l);
		}
		else {
			ann = ((NSG*) structure)->query(queries[q], N, lq);
		}
		clock_t end_ANN = clock();
		elapsed_secs_ANN += double(end_ANN - start_ANN) / CLOCKS_PER_SEC;

		clock_t start_TNN = clock();
		tuple<vector<int>, vector<double>> tnn = brute_force(dataset, queries[q], N);
		clock_t end_TNN = clock();
		elapsed_secs_TNN += double(end_TNN - start_TNN) / CLOCKS_PER_SEC;
		
        vector<int> indices_ann = get<0>(ann);
        vector<double> distances_ann = get<1>(ann);
        vector<int> indices_tnn = get<0>(tnn);
        vector<double> distances_tnn = get<1>(tnn);

		// Take the average approximate factor from all neighbors.
		double af = 0;
        for(int i = 0; (unsigned int) i < indices_ann.size(); i++){
			output << "Nearest neighbor-" << i+1 << ": " << indices_ann[i] << endl;
			output << "distanceApproximate: " << distances_ann[i] << endl;
			output << "distanceTrue: " << distances_tnn[i] << endl;
			af += distances_ann[i] / distances_tnn[i];
		}
		af /= indices_ann.size();
		aaf += af;
	}

	output << "tAverageApproximate: " << elapsed_secs_ANN / queries.size() << endl;
	output << "tAverageTrue: " << elapsed_secs_TNN / queries.size() << endl;
	output << "MAF: " << aaf / queries.size() << endl;
	
	output.close();
}