#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <tuple>
#include <set>
// iterator is used for std::back_insert_iterator, std::advance().

#include "gnn.hpp"
#include "mrng.hpp"
#include "lp_metric.hpp"
#include "brute_force.hpp"

using namespace std;

using std::vector;
using std::tuple;
using std::get;
using std::make_tuple;
using std::set;

#define distance euclidean_distance

// Writes the results of the queries to output file in the required format.
void handle_ouput(MRNG &mrng, const vector<vector<double>> &dataset, const vector<vector<double>> &queries, int N, int l, ofstream &output)
{
	for (int q = 0; q < (int) queries.size(); q++) {
		cout << "Query: " << q << endl;
		output << "Query: " << q << endl;
		clock_t start_ANN = clock();
		tuple<vector<int>, vector<double>> ann = mrng.query(queries[q], N, l, distance);
		clock_t end_ANN = clock();
		double elapsed_secs_ANN = double(end_ANN - start_ANN) / CLOCKS_PER_SEC;

		clock_t start_TNN = clock();
		tuple<vector<int>, vector<double>> tnn = brute_force(dataset, queries[q], N, distance);
		clock_t end_TNN = clock();
		double elapsed_secs_TNN = double(end_TNN - start_TNN) / CLOCKS_PER_SEC;
		
        vector<int> indices_ann = get<0>(ann);
        vector<double> distances_ann = get<1>(ann);
        vector<int> indices_tnn = get<0>(tnn);
        vector<double> distances_tnn = get<1>(tnn);
        for(int i = 0; (unsigned int) i < indices_ann.size(); i++){
			output << "Nearest neighbor-" << i+1 << ": " << indices_ann[i] << endl;
			output << "distanceApproximate: " << distances_ann[i] << endl;
			output << "distanceTrue: " << distances_tnn[i] << endl;
		}
		output << "tApproximate: " << elapsed_secs_ANN << endl;
		output << "tTrue: " << elapsed_secs_TNN << endl;
	}
	output.close();
}