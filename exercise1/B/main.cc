#include <iostream>
#include <vector>

#include "kmeanspp.h"

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
	
	vector<vector<double>> c = kmeanspp(p, k);

	// print the centroids
	for (int i = 0; i < (int) c.size(); i++) {
		cout << "c[" << i << "] = (";
		for (int j = 0; j < (int) c[i].size(); j++) {
			cout << c[i][j] << ", ";
		}
		cout << ")" << endl;
	}
}