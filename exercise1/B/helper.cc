#include <tuple>
#include <fstream>
#include <iostream>
#include <string>
#include <ctime>

#include "kmeans.h"

using namespace std;

// returns (K of K-means, L of LSH, k of LSH, M of Hypercube, k of Hypercube, probes of Hypercube
tuple<int, int, int, int, int, int> read_config_file(const string &filename)
{
	ifstream config_file(filename);
	int K_of_Kmeans = 10;
	int L = 3;
	int k_of_LSH = 4;
	int M = 10;
	int k_of_hypercube = 3;
	int probes = 2;

	// read line by line and parse and ignore // comments at begin or end of line
	string line;
	while (getline(config_file, line)) {
		if (line[0] == '#') {
			continue;
		}
		if (line.find("//") != string::npos) {
			line = line.substr(0, line.find("//"));
		}
		if (line.find("number_of_clusters:") != string::npos) {
			K_of_Kmeans = stoi(line.substr(line.find(":") + 1));
		}
		else if (line.find("number_of_vector_hash_functions:") != string::npos) {
			k_of_LSH = stoi(line.substr(line.find(":") + 1));
		}
		else if (line.find("number_of_vector_hash_tables:") != string::npos) {
			L = stoi(line.substr(line.find(":") + 1));
		}
		else if (line.find("number_of_hypercube_dimensions:") != string::npos) {
			k_of_hypercube = stoi(line.substr(line.find(":") + 1));
		}
		else if (line.find("number_of_probes:") != string::npos) {
			probes = stoi(line.substr(line.find(":") + 1));
		}
		else if (line.find("max_number_M_hypercube:") != string::npos) {
			M = stoi(line.substr(line.find(":") + 1));
		}
	}
	return make_tuple(K_of_Kmeans, L, k_of_LSH, M, k_of_hypercube, probes);
}

void handle_cluster_output(KMeans &kmeans, const string &output_file, bool complete, update_method method, const tuple<int, int, int, int, int, int> &config)
{
	ofstream output(output_file);
	switch (method) {
	case CLASSIC:
		output << "Algorithm: Lloyds" << endl;
		break;
	case REVERSE_LSH:
		output << "Algorithm: Range Search LSH" << endl;
		break;
	case REVERSE_HYPERCUBE:
		output << "Algorithm: Range Search Hypercube" << endl;
		break;
	}
	// set timer
	clock_t start = clock();
	tuple<int, int, int, int, int> kmean_args = make_tuple(get<1>(config), get<2>(config), get<3>(config), get<4>(config), get<5>(config));
	int k = get<0>(config);
	kmeans.compute_clusters(k, method, kmean_args);
	clock_t end = clock();
	// compute time
	double time = (double)(end - start) / CLOCKS_PER_SEC;
}