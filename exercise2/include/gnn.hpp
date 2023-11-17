#pragma once

#include <tuple>
#include <unordered_set>
#include <set>

#include "directed_graph.hpp"
#include "lsh.hpp"

class GNN
{
	private:
		DirectedGraph *G;
		LSH *lsh;
		const std::vector<std::vector<double>> &dataset;
		int R;
		int E;

		static bool cmp(const std::pair<int, double> *left, const std::pair<int, double> *right) { return left->second < right->second; }
		static bool hash(const std::pair<int, double> *p){ return std::hash<int>()(p->first); };
		static bool equal(const std::pair<int, double> *p1, const std::pair<int, double> *p2){ return p1->first == p2->first; };

		void add_neighbors_pred(int, std::vector<int>&, std::vector<double>&, int);
		void add_neighbors_random(int, std::vector<int>&, std::vector<double>&, int);

		void add_neighbors_pred(int, std::unordered_set<std::pair<int, double>*, decltype(&hash), decltype(&equal)>&, int);
		void add_neighbors_random(int, std::unordered_set<std::pair<int, double>*, decltype(&hash), decltype(&equal)>&, std::unordered_set<int>&, int);

	public:
		GNN(int k, const std::vector<std::vector<double>> &dataset, int R, int E);
		~GNN();
		std::tuple<std::vector<int>, std::vector<double>> query(const std::vector<double>&, unsigned int k,
																double (*distance)(const std::vector<double>&, const std::vector<double>&));
};