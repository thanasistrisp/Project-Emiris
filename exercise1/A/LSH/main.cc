#include <iostream>
#include <algorithm>
#include <tuple>
#include <cmath>

#include "lsh.h"

using namespace std;

double euclidean_distance(const std::vector<double>& v1, const std::vector<double>& v2){
	if(v1.size() != v2.size()){
        return -1;
    }

    double sum = 0.0;
    for(int i = 0; i < (int) v1.size(); i++){
        sum += pow(abs(v1.at(i) - v2.at(i)), 2);
    }
    return sqrt(sum);
}

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
	cout << "LSH test" << endl;

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

	int d = 3; // number of dimensions
	int n = 1000; // number of points
	int k = 3; // number of hash functions
	int l = 1; // number of hash tables
	int window = 4; // window

	cout << "\nBrute force solution: " << endl;
	brute_force(p, q, 10);

	cout << "\nLSH solution: " << endl;
	LSH lsh(d, n, k, l, window, &p);
	tuple<vector<int>, vector<double>> knn = lsh.query(q, 10, euclidean_distance);
	vector<int> indices = get<0>(knn);
	cout << indices.size() << endl;
	for (int i = 0; i < (int) indices.size(); i++) {
		cout << p.at(indices.at(i))[0] << " " << p.at(indices.at(i))[1] << " " << p.at(indices.at(i))[2] <<  endl;
	}
}