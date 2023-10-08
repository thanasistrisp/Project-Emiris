#include <iostream>
#include "hypercube.hpp"
#include <algorithm>

using namespace std;

// brute force nearest neighbor search
void brute_force(vector<vector<double>> p, vector<double> q, int K) { // find k exact nearest neighbors to q by brute force
	vector<pair<double, int>> distances(p.size());
	for (int i = 0; i < (int) p.size(); i++) {
		distances[i] = make_pair(euclidean_distance(p[i], q), i);
	}
	sort(distances.begin(), distances.end());
	for (int i = 0; i < K; i++) {
		cout << p[distances[i].second][0] << " " << p[distances[i].second][1] << " " << p[distances[i].second][2] << endl;
	}
}

int main(void) {
	// define 1000 points in 3D
	vector<vector<double>> p(1000, vector<double>(3));
	for (int i = 0; i < 1000; i++) {
		p[i][0] = i+2;
		p[i][1] = i+1;
		p[i][2] = i+1;
	}
	// define query point
	vector<double> q(3);
	q[0] = 500;
	q[1] = 250;
	q[2] = 970;
	// define k
	int k = 3; // log(1000)
	// define M
	int M = 100; // there are more than 100 in one vertex
	// define probes
	int probes = 2;
	// query
	vector<vector<double>> result = query(p, q, k, M, probes, 3, 100);
	// print result
	for (int i = 0; i < (int) result.size(); i++) {
		cout << result[i][0] << " " << result[i][1] << " " << result[i][2] << endl;
	}
	cout << "Brute force: " << endl;
	// brute force
	brute_force(p, q, 3);
	return 0;
}