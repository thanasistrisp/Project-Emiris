#include <iostream>
#include <vector>

#include "kmeans.h"

using namespace std;

int main(void) {
	// define 1000 points in 3D
	vector<vector<double>> p(10, vector<double>(3));
	for (int i = 0; i < 10; i++) {
		p[i][0] = i + 2;
		p[i][1] = i + 1;
		p[i][2] = i + 1;
	}

	int k = 3; // number of clusters
	
	KMeans kmeans(p);

	kmeans.compute_clusters(k, CLASSIC);

	vector<vector<double>> centroids = kmeans.get_centroids();
	vector<vector<int>> clusters = kmeans.get_clusters();

	for (int i = 0; i < k; i++) {
		cout << "Cluster " << i << endl;
		cout << "Centroid: ";
		for (int j = 0; j < (int) centroids[i].size(); j++) {
			cout << centroids[i][j] << ", ";
		}
		cout << endl;
		cout << "Points: ";
		for (int j = 0; j < (int) clusters[i].size(); j++) {
			cout << clusters[i][j] << " ";
		}
		cout << endl;
		// print also the points
		for (int j = 0; j < (int) clusters[i].size(); j++) {
			cout << "Point " << clusters[i][j] << ": ";
			for (int l = 0; l < (int) p[clusters[i][j]].size(); l++) {
				cout << p[clusters[i][j]][l] << ", ";
			}
			cout << endl;
		}
	}

	// check if the points are correctly assigned with the silhouette method
	for (int i = 0; i < (int) p.size(); i++) {
		cout << "s[" << i << "] = " << kmeans.silhouette(i) << endl;
	}
}