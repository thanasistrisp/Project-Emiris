#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <algorithm>
#include <map>
#include <random>
#include <tuple>

#include "metrics.hpp"
#include "h.hpp"
#include "hypercube.hpp"

using namespace std;

static int hamming_distance(vector<int> vertex, vector<int> q);
static bool same_vertex(vector<int> a, vector<int> b);

// return points indices packed in vertices, and returns vector of tuples of points indices in same vertex and the Hamming distance of vertex to query q
vector<vector<int>> hypercube::pack(vector<vector<int>> p_proj, int n, vector<int> q_proj, int hd) {
	vector<vector<int>> result;
	vector<int> points;
	for (int i = 0; i < n; i++) {
		int distance = hamming_distance(p_proj[i], q_proj);
		// add to points all points with distance == hd
		if (distance == hd) {
			points.push_back(i);
		}
	}

	// split points into vertices
	for (int i = 0; i < (int) points.size(); i++) {
		vector<int> vertex;
		vertex.push_back(points[i]);
		for (int j = i + 1; j < (int) points.size(); j++) {
			if (same_vertex(p_proj[points[i]], p_proj[points[j]])) {
				vertex.push_back(points[j]);
			}
		}
		result.push_back(vertex);
	}

	return result;
}

// preprocess: store points p -> [f-i(h_i(p))] for i = 1, ..., d'=k
vector<vector<int>> preprocess(vector<vector<double>> p, int k) {
	vector<vector<int>> result(p.size(), vector<int>(k));
	for (int i = 0; i < (int) p.size(); i++) {
		for (int j = 0; j < k; j++) {
			result[i][j] = f(h(p[i]));
		}
	}
	return result;
}


// compute hamming distance between a vertex and query q
static int hamming_distance(vector<int> vertex, vector<int> q) {
	int result = 0;
	for (int i = 0; i < (int) vertex.size(); i++) {
		if (vertex[i] != q[i])
			result++;
	}
	return result;
}

static bool same_vertex(vector<int> a, vector<int> b) {
	for (int i = 0; i < (int) a.size(); i++) {
		if (a[i] != b[i])
			return false;
	}
	return true;
}

int f(int x) { // uniform mapping to {0,1}
	if (x % 2 == 0)
		return 0;
	else
		return 1;
}