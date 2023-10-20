#include <iostream>
#include <vector>

#include "kmeans.h"

using namespace std;

int main(void) {
	// define 1000 points in 3D
	vector<vector<double>> p(1000, vector<double>(3));
	for (int i = 0; i < 1000; i++) {
		p[i][0] = i + 2;
		p[i][1] = i + 1;
		p[i][2] = i + 1;
	}

	int k = 3; // number of clusters
	
	KMeans kmeans(p);

	kmeans.compute_clusters(k, CLASSIC, vector<int>(), vector<double>());

	vector<vector<double>> centroids = kmeans.get_centroids();
	vector<vector<int>> clusters = kmeans.get_clusters();

	for (int i = 0; i < k; i++) {
		cout << "Cluster " << i << endl;
		cout << "Centroid: ";
		for (int j = 0; j < (int) centroids[i].size(); j++) {
			cout << centroids[i][j] << " ";
		}
		cout << endl;
		cout << "Points: ";
		for (int j = 0; j < (int) clusters[i].size(); j++) {
			cout << clusters[i][j] << " ";
		}
		cout << endl;
	}
}