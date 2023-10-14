#include <iostream>
#include <map>
#include <vector>
#include <limits>
#include <algorithm>

#include "metrics.hpp"
#include "hypercube.hpp"
#include "defines.hpp"

using namespace std;

hypercube::hypercube(std::vector<std::vector<double>> p, int k, int M, int probes, 
					 int N, double R, double (*distance)(std::vector<double>, std::vector<double> distance))
{
	this->p = p;
	this->k = k;
	this->M = M;
	this->probes = probes;
	this->N = N;
	this->R = R;
	this->distance = distance;
	
	clock_t start = clock();
	cout << "Preprocessing..." << endl;
	
	// Initialize h_i functions, i = 1, ..., k.
    HashFunction *h;
    for(int i = 0; i < k; i++){
        h = new HashFunction(p[0].size(), w);
        hash_functions.push_back(h);
    }

	// Initialize f_map
	f_map = new unordered_map<int, int>[k];

	p_proj = preprocess(p, k);

	clock_t end = clock();
	double elapsed_secs = double(end - start) / CLOCKS_PER_SEC;
	cout << "Preprocessing time: " << elapsed_secs << endl;
}

hypercube::~hypercube() {
	for (int i = 0; i < (int) hash_functions.size(); i++) {
		delete hash_functions[i];
	}	
	delete[] f_map;
}

vector<int> hypercube::query_n_nearest_neighbors(vector<double> q, vector<int> q_proj) {
	int num_points = 0;
	int num_vertices = 0;
	
	multimap<double, int> candidates; // allows duplicate keys sorted by distance in ascending order
	int hamming_distance = 0;
	while (true) {
		vector<vector<int>> vertices = pack(p_proj, (int) p_proj.size(), q_proj, hamming_distance);
		for (int i = 0; i < (int) vertices.size(); i++) {
			for (int j = 0; j < (int)vertices[i].size(); j++)
			{
				candidates.insert(pair<double, int>(distance(p[vertices[i][j]], q), vertices[i][j]));
				num_points++;
				if (num_points >= M)
					goto check;
			}
			num_vertices++;
			if (num_vertices >= probes)
				goto check;
		}
		hamming_distance++;
	}

	check:
		vector<int> nearest_neighbors(N);
		int i = 0;
		for (auto it = candidates.begin(); it != candidates.end(); it++) {
			if (i >= N) {
				break;
			}
			nearest_neighbors[i] = it->second;
			i++;
		}
		return nearest_neighbors;
}

vector<int> hypercube::query_range(vector<double> q, vector<int> q_proj) {
	int num_points = 0;
	int num_vertices = 0;

	multimap<double, int> candidates;
	int hamming_distance = 0;
	while (true) {
		vector<vector<int>> vertices = pack(p_proj, (int) p_proj.size(), q_proj, hamming_distance);
		for (int i = 0; i < (int) vertices.size(); i++) {
			for (int j = 0; j < (int)vertices[i].size(); j++)
			{
				if (distance(p[vertices[i][j]], q) < R)
				{
					candidates.insert(pair<double, int>(distance(p[vertices[i][j]], q), vertices[i][j]));
				}
				num_points++;
				if (num_points >= M)
					goto check;
			}
			num_vertices++;
			if (num_vertices >= probes)
				goto check;
		}
		hamming_distance++;
	}

	check:
		vector<int> range;
		for (auto it = candidates.begin(); it != candidates.end(); it++) {
			range.push_back(it->second);
		}
		return range;
}

vector<int> hypercube::calculate_q_proj(vector<double> q) {
	vector<int> q_proj;
	for (int i = 0; i < k; i++) {
		q_proj.push_back(f(hash_functions[i]->hash(q), i));
	}
	return q_proj;
}

// preprocess: store points p -> [f-i(h_i(p))] for i = 1, ..., d'=k
vector<vector<int>> hypercube::preprocess(vector<vector<double>> p, int k) {
	vector<vector<int>> result(p.size(), vector<int>(k));
	for (int i = 0; i < (int) p.size(); i++) {
		for (int j = 0; j < k; j++) {
			int h_j = hash_functions[j]->hash(p[i]);
			result[i][j] = f(h_j, j);
		}
	}
	return result;
}