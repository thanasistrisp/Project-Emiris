#include <iostream>
#include "hypercube.hpp"
#include <algorithm>

using namespace std;

// brute force nearest neighbor search
void brute_force(vector<vector<double>> p, vector<double> q) {
	// compute distance of q to every point in p
	vector<double> distances(p.size());
	for (int i = 0; i < (int) p.size(); i++) {
		distances[i] = euclidean_distance(p[i], q);
	}
	// sort distances
	sort(distances.begin(), distances.end());
	// print nearest neighbor point in 3d
	for (int i = 0; i < (int) p.size(); i++) {
		if (euclidean_distance(p[i], q) == distances[0]) { // print all of them if equal
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
	int M = 20; // there are more than 100 in one vertex
	// define probes
	int probes = 2;
	// query
	vector<double> result = query(p, q, k, M, probes);
	// print result
	for (int i = 0; i < (int) result.size(); i++) {
		cout << result[i] << " ";
	}
	cout << endl;
	brute_force(p, q);
}