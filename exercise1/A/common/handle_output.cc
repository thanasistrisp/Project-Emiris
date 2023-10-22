#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <fstream>
#include <limits>
#include <algorithm>

#include "hypercube.hpp"
#include "helper.hpp"

using namespace std;

// brute force search for nearest neighbors
tuple<vector<int>, vector<double>> brute_force(vector<vector<double>> dataset, vector<double> query, unsigned int N, double (*distance)(const vector<double>&, const vector<double>&))
{
	auto compare = [](tuple<int, double> t1, tuple<int, double> t2){ return get<1>(t1) < get<1>(t2); };
	set<tuple<int, double>, decltype(compare)> s(compare);

	double dist;
	for(int i = 0; (unsigned int) i < dataset.size(); i++){
		dist = distance(dataset[i], query);
		if(s.size() == N){
			if(dist >= get<1>(*s.rbegin())){
				continue;
			}
			s.erase(std::prev(s.end()));
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

void handle_ouput(hypercube &cube, ofstream &output, const vector<vector<double>> &queries, double R, int N)
{
	for (int q = 0; q < (int) queries.size(); q++) {
		vector<vector<double>> dataset = cube.get_dataset();
		vector<int> q_proj = cube.calculate_q_proj(queries[q]);
		cout << "Query: " << q << endl;
		output << "Query: " << q << endl;
		clock_t start_ANN = clock();
		vector<int> n_nearest_neighbors;
		vector<double> dist_ann;
		tie(n_nearest_neighbors, dist_ann) = cube.query_n_nearest_neighbors(queries[q], q_proj, N);
		clock_t end_ANN = clock();
		double elapsed_secs_ANN = double(end_ANN - start_ANN) / CLOCKS_PER_SEC;

		clock_t start_ENN = clock();
		vector<int> real_nearest_neighbors;
		vector<double> dist_true;
		tie(real_nearest_neighbors, dist_true) = brute_force(dataset, queries[q], N, cube.distance);
		clock_t end_ENN = clock();
		double elapsed_secs_ENN = double(end_ENN - start_ENN) / CLOCKS_PER_SEC;
		
		for (int i = 0; i < N; i++) {
			output << "Nearest neighbor-" << i+1 << ": " << n_nearest_neighbors[i] << endl;
			output << "distanceHypercube: " << cube.distance(dataset[n_nearest_neighbors[i]], queries[q]) << endl;
			output << "distanceTrue: " << dist_true[i] << endl;
		}
		output << "tHypercube: " << elapsed_secs_ANN << endl;
		output << "tTrue: " << elapsed_secs_ENN << endl;
		output << "R-near neighbors:" << endl;
		vector<int> range_neighbors = cube.query_range(queries[q], q_proj, R);
		for (int i = 0; i < (int) range_neighbors.size(); i++) {
			output << range_neighbors[i] << endl;
		}
	}

	output.close();
}