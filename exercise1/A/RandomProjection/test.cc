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

// brute force radius search sorted by distance
void brute_force_R(vector<vector<double>> p, vector<double> q, double R) {
	vector<pair<double, int>> distances(p.size());
	for (int i = 0; i < (int) p.size(); i++) {
		distances[i] = make_pair(euclidean_distance(p[i], q), i);
	}
	sort(distances.begin(), distances.end());
	int counter = 0;
	for (int i = 0; i < (int) distances.size(); i++) {
		if (distances[i].first > R) {
			break;
		}
		cout << p[distances[i].second][0] << " " << p[distances[i].second][1] << " " << p[distances[i].second][2] << endl;
		counter++;
	}
	cout << "Number of points in range: " << counter << endl;
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
	q[0] = 501;
	q[1] = 500;
	q[2] = 500;
	// define k
	int k = 3; // log(1000)
	// define M
	int M = 1000; // there are more than 100 in one vertex
	// define probes
	int probes = 1000;
	// define Radius
	double R = 520;
	// define nearest neighbors
	int N = 4;
	// define hypercube
	hypercube hc(p, k, M, probes, N, R);
	// call query
	tuple<vector<int>, vector<double>> result = hc.query_n_nearest_neighbors(q, hc.calculate_q_proj(q));
	vector<int> nearest_neighbors = get<0>(result);
	vector<double> dist = get<1>(result);
	// print results
	cout << "Nearest neighbors:" << endl;
	for (int i = 0; i < (int) nearest_neighbors.size(); i++) {
		cout << p[nearest_neighbors[i]][0] << " " << p[nearest_neighbors[i]][1] << " " << p[nearest_neighbors[i]][2] << endl;
	}
	cout << endl;
	// call query
	vector<int> range = hc.query_range(q, hc.calculate_q_proj(q));
	// print results
	int counter = 0;
	cout << "Range:" << endl;
	for (int i = 0; i < (int) range.size(); i++) {
		cout << p[range[i]][0] << " " << p[range[i]][1] << " " << p[range[i]][2] << endl;
		counter++;
	}
	cout << "Number of points in range: " << counter << endl;
	cout << endl;
	// call brute force
	cout << "Brute force nearest neighbors:" << endl;
	brute_force(p, q, N);
	cout << endl;
	// call brute force
	cout << "Brute force range:" << endl;
	brute_force_R(p, q, R);
	cout << endl;

}