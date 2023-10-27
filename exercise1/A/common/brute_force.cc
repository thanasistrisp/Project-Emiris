#include <vector>
#include <tuple>
#include <set>

using namespace std;

// Returns the indices of the k-exact nearest neighbours (k-NN) of the given query q
// and their distances to the query based on the given distance function.
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