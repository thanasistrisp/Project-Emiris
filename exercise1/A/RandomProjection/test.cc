#include <iostream>
#include "hypercube.hpp"
#include <algorithm>

using namespace std;

// brute force nearest neighbor search
void brute_force(vector<vector<double>> p, vector<double> q, int K) {
	vector<pair<double, int>> distances(p.size());
	for (int i = 0; i < (int) p.size(); i++) {
		distances[i] = make_pair(euclidean_distance(p[i], q), i);
	}
	sort(distances.begin(), distances.end());
	for (int i = 0; i < K; i++) {
		cout << p[distances[i].second][0] << " " << p[distances[i].second][1] << " " << p[distances[i].second][2] << endl;
	}
}

// brute force radius search
void brute_force(vector<vector<double>> p, vector<double> q, double R) {
	for (int i = 0; i < (int) p.size(); i++) {
		if (euclidean_distance(p[i], q) <= R) {
			cout << p[i][0] << " " << p[i][1] << " " << p[i][2] << endl;
		}
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
	// define Radius
	double R = 520;
	// define nearest neighbors
	int N = 3;
	// define hypercube
	hypercube hypercube(p, q, k, M, probes, N, R);
	// query
	vector<vector<double>> k_candidates = hypercube.query_n_nearest_neighbors();
	cout << "k_candidates: " << endl;
	for (int i = 0; i < (int) k_candidates.size(); i++) {
		cout << k_candidates[i][0] << " " << k_candidates[i][1] << " " << k_candidates[i][2] << endl;
	}
	cout << "\nBrute force: " << endl;
	// brute force
	brute_force(p, q, 3);
	cout << "\nr_candidates: " << endl;
	vector<vector<double>> r_candidates = hypercube.query_range();
	for (int i = 0; i < (int) r_candidates.size(); i++) {
		cout << r_candidates[i][0] << " " << r_candidates[i][1] << " " << r_candidates[i][2] << endl;
	}
	cout << "\nBrute force: " << endl;
	brute_force(p, q, R);
	return 0;
}