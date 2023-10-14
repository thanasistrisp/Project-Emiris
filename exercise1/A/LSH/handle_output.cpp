#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <tuple>
#include <set>
// iterator is used for std::back_insert_iterator, std::advance().

#include "lsh.h"
#include "helper.hpp"
#include "lp_metric.h"

using namespace std;

using std::vector;
using std::tuple;
using std::get;
using std::make_tuple;
using std::set;

// brute force search for nearest neighbors
tuple<vector<int>, vector<double>> brute_force(vector<vector<double>> dataset, vector<double> query, unsigned int N, double (*distance)(const vector<double>&, const vector<double>&))
{
	auto compare = [](tuple<int, double> t1, tuple<int, double> t2){ return get<1>(t1) < get<1>(t2); };
	set<tuple<int, double>, decltype(compare)> s(compare);

	double dist;
	for(int i = 0; (unsigned int) i < dataset.size(); i++){
		dist = distance(dataset[i], query);
		if(s.size() == N){
			if(dist >= get<1>(*s.begin())){
				continue;
			}
			s.erase(s.begin());
		}
		s.insert(make_tuple(i, dist));
	}

	vector<int> indices;
	vector<double> distances;
	set<tuple<int, double>>::const_iterator iter;
	for(iter = s.begin(); iter != s.end(); std::advance(iter, 1)){
		indices.push_back(get<0>(*iter));
		distances.push_back(get<1>(*iter));
	}
	return make_tuple(indices, distances);
}

void handle_ouput(LSH &lsh, const vector<vector<double>> &dataset, const vector<vector<double>> &queries, int n, ofstream &output) {
	for (int q = 0; q < (int) queries.size(); q++) {
		cout << "Query: " << q << endl;
		output << "Query: " << q << endl;
		clock_t start_ANN = clock();
		tuple<vector<int>, vector<double>> ann = lsh.query(queries[q], n, euclidean_distance);
		clock_t end_ANN = clock();
		double elapsed_secs_ANN = double(end_ANN - start_ANN) / CLOCKS_PER_SEC;

		clock_t start_TNN = clock();
		tuple<vector<int>, vector<double>> tnn = brute_force(dataset, queries[q], n, euclidean_distance);
		clock_t end_TNN = clock();
		double elapsed_secs_TNN = double(end_TNN - start_TNN) / CLOCKS_PER_SEC;
		
        vector<int> indices_ann = get<0>(ann);
        vector<double> distances_ann = get<1>(ann);
        vector<int> indices_tnn = get<0>(tnn);
        vector<double> distances_tnn = get<1>(tnn);
        for(int i = 0; (unsigned int) i < indices_ann.size(); i++){
			output << "Nearest neighbor-" << i+1 << ": " << indices_ann[i] << endl;
			output << "distanceLSH: " << distances_ann[i] << endl;
			output << "distanceTrue: " << distances_tnn[i] << endl;
		}
		output << "tLSH: " << elapsed_secs_ANN << endl;
		output << "tTrue: " << elapsed_secs_TNN << endl;
		// output << "R-near neighbors:" << endl;
		// vector<int> range_neighbors = cube.query_range(queries[q], q_proj);
		// for (int i = 0; i < (int) range_neighbors.size(); i++) {
		// 	output << range_neighbors[i] << endl;
		// }
	}

	output.close();
}