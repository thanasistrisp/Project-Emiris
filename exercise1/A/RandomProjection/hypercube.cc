#include <iostream>
#include <map>
#include <vector>
#include <limits>
#include <algorithm>

#include "lp_metric.hpp"
#include "hypercube.hpp"
#include "defines.hpp"

using namespace std;

hypercube::hypercube(const vector<vector<double>> &p, int k, int M, int probes, 
					 double (*distance)(const std::vector<double> &, const std::vector<double> &)) : p(p)
{
	this->k = k;
	this->M = M;
	this->probes = probes;
	this->distance = distance;
	
	clock_t start = clock();
	cout << "Preprocessing..." << endl;
	
	// Initialize h_i functions, i = 1, ..., k.
    HashFunction *h;
    for(int i = 0; i < k; i++){
        h = new HashFunction(p[0].size(), w);
        hash_functions.push_back(h);
    }

	// Initialize f_map so that that f_i(j) is calculated only once for specific j but could be different for different i
	f_map = new unordered_map<int, int>[k];

	// initialize HashTable
	hash_table = new HashTable();
	// for each point p, calculate h_i(p) and store p -> [f-i(h_i(p))] for i = 1, ..., d'=k in hash_table
	for (int i = 0; i < (int) p.size(); i++) {
		vector<int> p_proj;
		for (int j = 0; j < k; j++) {
			int h_j = hash_functions[j]->hash(p[i]);
			p_proj.push_back(f(h_j, j));
		}
		binary_string bs(p_proj); // convert p_proj to binary string type
		auto it = hash_table->find(bs); // check if bs exists in hash_table
		if (it != hash_table->end()) { // if it exists, add i to bucket of bs
			it->second.push_back(i);
		}
		else { // if not, create new bucket
			vector<int> v;
			v.push_back(i);
			hash_table->insert(pair<binary_string, vector<int>>(bs, v));
		}
	}

	// initialize used_vertices
	used_vertices = new unordered_set<binary_string, binary_string::hash>();

	clock_t end = clock();
	double elapsed_secs = double(end - start) / CLOCKS_PER_SEC;
	cout << "Preprocessing time: " << elapsed_secs << endl;
}

hypercube::~hypercube() {
	for (int i = 0; i < (int) hash_functions.size(); i++) {
		delete hash_functions[i];
	}	
	delete[] f_map;
	delete hash_table;
	delete used_vertices;
}

tuple<vector<int>, vector<double>> hypercube::query_n_nearest_neighbors(const vector<double> &q, const vector<int> &q_proj, int N) {
	int num_points = 0;
	int num_vertices = 0;
	
	// initialize N best candidates and distances
	vector<int> best_candidates(N);
	vector<double> best_distances(N, numeric_limits<double>::max());

	int hamming_distance = 0;
	while (true) {
		// create all permutations of q_proj with hamming_distance = 0, 1, 2, ...
		vector<vector<int>> vertices = pack(q_proj, hamming_distance);
		for (int i = 0; i < (int) vertices.size(); i++) {
			for (int j = 0; j < (int)vertices[i].size(); j++)
			{
				if (num_points >= M)
					goto check;
				double dist = distance(p[vertices[i][j]], q);
				if (dist < best_distances[N - 1]) {
					best_distances[N - 1] = dist;
					best_candidates[N - 1] = vertices[i][j];
					// sort best_distances and best_candidates
					for (int k = N - 1; k > 0; k--) { // insertion sort (N is small)
						if (best_distances[k] < best_distances[k - 1]) {
							swap(best_distances[k], best_distances[k - 1]);
							swap(best_candidates[k], best_candidates[k - 1]);
						}
						else {
							break;
						}
					}
				}
				num_points++;
			}
			num_vertices++;
			if (num_vertices >= probes)
				goto check;
		}
		hamming_distance++;
	}

	check:
		// convert multimap to vector to match the return type
		vector<int> nearest_neighbors;
		vector<double> dist;
		for (int i = 0; i < N; i++) {
			nearest_neighbors.push_back(best_candidates[i]);
			dist.push_back(best_distances[i]);
		}
		// reset used_vertices
		used_vertices->clear();

		return make_tuple(nearest_neighbors, dist);
}

tuple<vector<int>, vector<double>> hypercube::query_range(const vector<double> &q, const vector<int> &q_proj, double R) {
	int num_points = 0;
	int num_vertices = 0;

	multimap<double, int> candidates; // used multimap to sort candidates by distance and keep duplicates
	int hamming_distance = 0;
	while (true) {
		// create all permutations of q_proj with hamming_distance = 0, 1, 2, ...
		vector<vector<int>> vertices = pack(q_proj, hamming_distance);
		for (int i = 0; i < (int) vertices.size(); i++) {
			for (int j = 0; j < (int)vertices[i].size(); j++)
			{
				if (num_points >= M)
					goto check;
				if (distance(p[vertices[i][j]], q) < R)
				{
					candidates.insert(pair<double, int>(distance(p[vertices[i][j]], q), vertices[i][j]));
				}
				num_points++;
			}
			num_vertices++;
			if (num_vertices >= probes)
				goto check;
		}
		hamming_distance++;
	}

	check:
		// convert multimap to vector to match the return type
		vector<int> range;
		vector<double> dist;
		for (auto it = candidates.begin(); it != candidates.end(); it++) {
			range.push_back(it->second);
			dist.push_back(it->first);
		}
		// reset used_vertices
		used_vertices->clear();

		return make_tuple(range, dist);
}

vector<int> hypercube::calculate_q_proj(const vector<double> &q) {
	vector<int> q_proj;
	for (int i = 0; i < k; i++) {
		q_proj.push_back(f(hash_functions[i]->hash(q), i));
	}
	return q_proj;
}