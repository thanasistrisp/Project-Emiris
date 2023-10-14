#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <algorithm>
#include <map>
#include <random>
#include <tuple>

#include "metrics.hpp"
#include "hypercube.hpp"

using namespace std;

static int hamming_distance(const vector<int> &vertex, const vector<int> &q);
static bool same_vertex(const vector<int> &a, const vector<int> &b);

// return points indices packed in vertices, and returns vector of tuples of points indices in same vertex and the Hamming distance of vertex to query q
vector<vector<int>> hypercube::pack(const vector<vector<int>> &p_proj, int n, const vector<int> &q_proj, int hd) {
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
		bool found = false;
		for (int j = 0; j < (int) result.size(); j++) {
			if (same_vertex(p_proj[points[i]], p_proj[result[j][0]])) {
				result[j].push_back(points[i]);
				found = true;
				break;
			}
		}
		if (!found) {
			vector<int> new_vertex;
			new_vertex.push_back(points[i]);
			result.push_back(new_vertex);
		}
	}

	return result;
}

// compute hamming distance between a vertex and query q
static int hamming_distance(const vector<int> &vertex, const vector<int> &q) {
	int result = 0;
	for (int i = 0; i < (int) vertex.size(); i++) {
		if (vertex[i] != q[i])
			result++;
	}
	return result;
}

static bool same_vertex(const vector<int> &a, const vector<int> &b) {
	for (int i = 0; i < (int) a.size(); i++) {
		if (a[i] != b[i])
			return false;
	}
	return true;
}

int hypercube::f(int x, int i) {
	// if x not in f_map, randomly generate 0 or 1, store in f_map and return, else return f_map[x]
	if (f_map[i].find(x) == f_map[i].end()) {
		f_map[i][x] = rand() % 2;
	}
	return f_map[i][x];
}