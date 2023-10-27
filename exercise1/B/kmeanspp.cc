#include <iostream>
#include <vector>
#include <random>
#include <algorithm>

#include "kmeans.hpp"

using namespace std;

static vector<double> calculate_D(const vector<vector<double>> &p, const vector<vector<double>> &c);
static vector<double> calculate_P(const vector<double> &D);
static void normalize_vector(vector<double> &v);
static int binary_search(const vector<double> &p, double x);


void KMeans::kmeanspp() {
	// deep copy of the dataset using copy constructor
	vector<vector<double>> p(dataset);
	random_device rd;
	default_random_engine random_engine(rd());
	uniform_int_distribution<int> distribution(0, p.size() - 1);
	int i = distribution(random_engine);
	centroids.push_back(p[i]);
	// delete the centroid from the list of points
	p.erase(p.begin() + i);
	// calculate the distance from each point to the centroid
	for (int t = 1; t < (int) clusters.size(); t++) {
		vector<double> D = calculate_D(p, centroids);
		normalize_vector(D);
		vector<double> P = calculate_P(D);
		sort(P.begin(), P.end());
		random_device rd;
		default_random_engine random_engine(rd());
		uniform_real_distribution<double> distribution(0, P[P.size() - 1]);
		double x = distribution(random_engine);
		int r = binary_search(P, x); // find and return r such that P[r-1] < x <= P[r]
		centroids.push_back(p[r]); // add the centroid to the list of centroids
		p.erase(p.begin() + r); // delete the centroid from the list of points
	}
}


// Helper functions

// calculate the distance of each point to the nearest centroid
static vector<double> calculate_D(const vector<vector<double>> &p, const vector<vector<double>> &c) {
	vector<double> D(p.size());
	for (int i = 0; i < (int) p.size(); i++) {
		double min = KMeans::distance(p[i], c[0]);
		for (int j = 1; j < (int) c.size(); j++) {
			double d = KMeans::distance(p[i], c[j]);
			if (d < min) {
				min = d;
			}
		}
		D[i] = min;
	}
	return D;
}

// calculate the cumulative probability distribution of D
static vector<double> calculate_P(const vector<double> &D) {
	vector<double> P(D.size());
	double sum = 0;
	int r = D.size();
	for (int i = 0; i < r; i++) {
		sum += D[i];
		P[i] = sum;
	}
	return P;
}

// normalize the given vector
static void normalize_vector(vector<double> &v) {
	double max = v[0];
	for (int i = 1; i < (int) v.size(); i++) {
		if (v[i] > max) {
			max = v[i];
		}
	}
	for (int i = 0; i < (int) v.size(); i++) {
		v[i] /= max;
	}
}

// binary search in already sorted vector
static int binary_search(const vector<double> &p, double x) {
	int l = 0;
	int r = p.size() - 1;
	while (l < r) {
		int m = (l + r) / 2;
		if (p[m] < x) {
			l = m + 1;
		} else {
			r = m;
		}
	}
	return l;
}