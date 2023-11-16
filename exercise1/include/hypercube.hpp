#pragma once

#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include "lp_metric.hpp"
#include "hash_function.hpp"
#include "binary_string.hpp"
#include "defines.hpp"

class hypercube
{
private:
	const std::vector<std::vector<point>> &p; // Dataset.
	int k;      // Number of hash functions.
	int M;      // Maximum number of candidate data points checked.
	int probes; // Maximum number of hypercube vertices checked (probes).

	std::unordered_map<int, int> *f_map;

	// Set of vertices that have been used in the query so far.
	std::unordered_set<binary_string, binary_string::hash> *remaining_vertices;

	// Define hash table type that maps binary strings to vectors of indices.
	typedef std::unordered_map<binary_string, std::vector<int>, binary_string::hash> HashTable;
	HashTable *hash_table;

	// Hash functions h_i, i = 1, ..., k.
	std::vector<HashFunction*> hash_functions;

	// Define f_i(x) = 0 or 1.
	int f(int x, int i);

	// Returns the vertices that are at hamming distance from q_proj.
	std::vector<std::vector<int>> pack(const std::vector<int> &q_proj, int hamming_distance);

public:
	// Initializes an instance with the given dataset, number of dimensions k, maximum number of candidate data points checked,
	// maximum number of hypercube vertices checked (probes) based on the given distance function.
	hypercube(const std::vector<std::vector<point>> &p, int k, int M, int probes, 
			  double (*distance)(const std::vector<point> &, const std::vector<point> &) = euclidean_distance);
	~hypercube();

	// Returns the indices of the N nearest neighbours of q and their distances to q.
	std::tuple<std::vector<int>, std::vector<double>> query_n_nearest_neighbors(const std::vector<point> &q, const std::vector<int> &q_proj, int N);
	
	// Returns the indices of the neighbours of q that lie within radius R and their distances to q.
	std::tuple<std::vector<int>, std::vector<double>> query_range(const std::vector<point> &q, const std::vector<int> &q_proj, double R);
	
	// Returns the projection of q.
	std::vector<int> calculate_q_proj(const std::vector<point> &q);

	std::vector<std::vector<point>> get_dataset() { return p; }
	
	// Distance function.
	double (*distance)(const std::vector<point> &, const std::vector<point> &);
};