#include <tuple>
#include <fstream>
#include <iostream>
#include <string>
#include <ctime>

#include "kmeans.hpp"

using namespace std;

tuple<int, int, int, int, int, int, double> read_config_file(const string &filename)
{
	ifstream config_file(filename);

	// Default values.
	int K_of_Kmeans = 10;
	int L = 3;
	int k_of_LSH = 4;
	int M = 10;
	int k_of_hypercube = 3;
	int probes = 2;
	double window = 1000;

	// Read line by line and parse and ignore // comments at begin or end of line.
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
		else if (line.find("window:") != string::npos) {
			window = stod(line.substr(line.find(":") + 1));
		}
	}
	return make_tuple(K_of_Kmeans, L, k_of_LSH, M, k_of_hypercube, probes, window);
}

void handle_cluster_output(KMeans &kmeans, const string &output_file, bool complete, update_method method, const tuple<int, int, int, int, int, int, double> &config)
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
	cout << "Running clustering algorithm..." << endl;
	clock_t start = clock();
	tuple<int, int, int, int, int, double> kmean_args = make_tuple(get<1>(config), get<2>(config), get<3>(config), get<4>(config), get<5>(config), get<6>(config));
	int k = get<0>(config);
	kmeans.compute_clusters(k, method, kmean_args);
	clock_t end = clock();
	double time = (double)(end - start) / CLOCKS_PER_SEC;
	cout << "Clustering time: " << time << endl;

	vector<vector<int>> clusters = kmeans.get_clusters();
	vector<vector<double>> centroids = kmeans.get_centroids();

	for (int i = 0; i < k; i++) {
		output << "CLUSTER-" << i + 1 << " {size: " << clusters[i].size() << ", centroid: ";
		for (int j = 0; j < (int)centroids[i].size(); j++) {
			output << centroids[i][j];
			if (j != (int)centroids[i].size() - 1) {
				output << ", ";
			}
		}
		output << "}" << endl;
	}
	output << "clustering_time: " << time << endl;

	clock_t start_silhouette = clock();
	cout << "Computing silhouette..." << endl;
	output << "Silhouette: [";
	vector<double> si(clusters.size(), 0);
	double stotal = 0;
	for (int i = 0; i < (int) clusters.size(); i++) { // For each cluster.
		for (int j = 0; j < (int) clusters[i].size(); j++) { // For each point in cluster.
			si[i] += kmeans.silhouette(clusters[i][j]); // Add silhouette of point to cluster silhouette.
		}
		stotal += si[i]; // Add cluster silhouette to total silhouette.
		si[i] /= clusters[i].size();
	}
	stotal /= kmeans.get_dataset_size(); // Calculate average silhouette of dataset (average of cluster silhouettes).
	for (int i = 0; i < (int) si.size(); i++) {
		output << si[i];
		output << ", ";
	}
	output << stotal << "]" << endl;
	clock_t end_silhouette = clock();
	double silhouette_time = (double)(end_silhouette - start_silhouette) / CLOCKS_PER_SEC;
	cout << "Silhouette time: " << silhouette_time << endl;

	if (complete) {
		for (int i = 0; i < (int) clusters.size(); i++) {
			output << "CLUSTER-" << i + 1 << " {";
			for (int j = 0; j < (int) centroids[i].size(); j++) {
				output << centroids[i][j];
				if (j != (int) centroids[i].size() - 1) {
					output << ", ";
				}
			}
			for (int j = 0; j < (int) clusters[i].size(); j++) {
				output << clusters[i][j];
				if (j != (int) clusters[i].size() - 1) {
					output << ", ";
				}
			}
			output << "}" << endl;
		}
		
	}
}