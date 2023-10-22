#include <iostream>

#include "kmeans.h"
#include "helper.hpp"

using namespace std;

int main() {
	vector<vector<double>> dataset = read_mnist_data("../MNIST/train-images-idx3-ubyte");
	dataset.resize(1000);
	KMeans kmeans(dataset);
	// start time
	clock_t begin = clock();
	auto params = make_tuple(0, 0, 0, 0, 0);
	kmeans.compute_clusters(10, CLASSIC, params);
	// end time
	clock_t end = clock();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	cout << "Time elapsed: " << elapsed_secs << endl;
	vector<vector<double>> centroids = kmeans.get_centroids();
	vector<vector<int>> clusters = kmeans.get_clusters();
	// print images to output folder in folder for each cluster
	for (int i = 0; i < (int) clusters.size(); i++) {
		for (int j = 0; j < (int) clusters[i].size(); j++) {
			export_image(dataset[clusters[i][j]], "../output/clustering/cluster_" + to_string(i) + "/image_" + to_string(j) + ".png");
		}
	}
	cout << "Done!" << endl;
	// si = average of points in cluster i (silhouette)
	vector<double> si(clusters.size(), 0);
	double stotal = 0;
	for (int i = 0; i < (int) clusters.size(); i++) {
		for (int j = 0; j < (int) clusters[i].size(); j++) {
			si[i] += kmeans.silhouette(clusters[i][j]);
		}
		stotal += si[i];
		si[i] /= clusters[i].size();
	}
	stotal /= dataset.size();
	// print results
	cout << "Silhouette for each cluster:" << endl;
	for (int i = 0; i < (int) si.size(); i++) {
		cout << "Cluster " << i << ": " << si[i] << endl;
	}
	cout << "Total silhouette: " << stotal << endl;
}