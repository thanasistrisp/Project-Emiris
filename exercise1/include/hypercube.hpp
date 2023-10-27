#pragma once

#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include "lp_metric.hpp"
#include "hash_function.hpp"
#include "binary_string.hpp"

class hypercube
{
private:
	const std::vector<std::vector<double>> &p;
	int k;
	int M;
	int probes;

	std::unordered_map<int, int> *f_map;

	// used in permutations calculation to avoid duplicates
	std::unordered_set<binary_string, binary_string::hash> *used_vertices;

	// define hash table type that maps binary strings to vectors of indices
	typedef std::unordered_map<binary_string, std::vector<int>, binary_string::hash> HashTable;
	HashTable *hash_table;

	// hash functions h_i, i = 1, ..., k
	std::vector<HashFunction*> hash_functions;
	// define f_i(x) = 0 or 1
	int f(int x, int i);

	// returns the vertices that are at hamming distance from q_proj
	std::vector<std::vector<int>> pack(const std::vector<int> &q_proj, int hamming_distance);
	// returns all permutations of q_proj with hamming distance
	std::set<std::vector<int>> get_permutation(const std::vector<int> &q_proj, int hamming_distance);
public:
	hypercube(const std::vector<std::vector<double>> &p, int k, int M, int probes, 
			  double (*distance)(const std::vector<double> &, const std::vector<double> &) = euclidean_distance);
	~hypercube();
	// returns the indices of the N nearest neighbours of q and their distances to q
	std::tuple<std::vector<int>, std::vector<double>> query_n_nearest_neighbors(const std::vector<double> &q, const std::vector<int> &q_proj, int N);
	// returns the indices of the neighbours of q that lie within radius R and their distances to q
	std::tuple<std::vector<int>, std::vector<double>> query_range(const std::vector<double> &q, const std::vector<int> &q_proj, double R);
	// returns the projection of q
	std::vector<int> calculate_q_proj(const std::vector<double> &q);
	std::vector<std::vector<double>> get_dataset() { return p; }
	// distance function
	double (*distance)(const std::vector<double> &, const std::vector<double> &);
};