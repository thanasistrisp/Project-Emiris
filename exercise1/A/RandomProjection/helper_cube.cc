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
vector<vector<int>> pack(vector<vector<int>> p_proj, int n, vector<int> q_proj) {
	vector<tuple<vector<int>, int>> result;
	// add points that are in same vertex in same tuple (e.g. points 1,5,6 with distance 2)
	// define used point (e.g. used[p] = true if point p is already in a tuple) (map)
	map<vector<int>, bool> used;
	for (int i = 0; i < (int) p_proj.size(); i++) { // it does not accept duplicate keys
		used[p_proj[i]] = false;
	}
	for (int i = 0; i < n; i++) {
		if (used[p_proj[i]] == false) {
			vector<int> vertex;
			int distance = hamming_distance(p_proj[i], q_proj);
			for (int j = 0; j < n; j++) {
				if (same_vertex(p_proj[i], p_proj[j])) {
					vertex.push_back(j);
					used[p_proj[j]] = true;
				}
			}
			result.push_back(make_tuple(vertex, distance));
		}
	}
	// sort tuples by Hamming distance
	sort(result.begin(), result.end(), [](tuple<vector<int>, int> a, tuple<vector<int>, int> b) {
		return get<1>(a) < get<1>(b);
	});
	// return vector of vertices without Hamming distance
	vector<vector<int>> vertices;
	for (int i = 0; i < (int) result.size(); i++) {
		vertices.push_back(get<0>(result[i]));
	}
	return vertices;	
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