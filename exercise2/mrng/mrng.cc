#include <tuple>
#include <set>
#include <vector>
#include <algorithm>
#include <map>
#include <unordered_set>

#include "mrng.hpp"
#include "lsh.hpp"
#include "defines.hpp"
#include "lp_metric.hpp"

using namespace std;

MRNG::MRNG(int k, const vector<vector<double>> &dataset, int R, int E): dataset(dataset), R(R), E(E)
{
	k++;
    R++;
    E++;
}

MRNG::~MRNG()
{
	
}

tuple<vector<int>, vector<double>> MRNG::query(const vector<double>& q, unsigned int N,
                                              double (*distance)(const vector<double>&, const vector<double>&))
{
    vector<double> v1 = vector<double>();
    vector<double> v2 = vector<double>();
    distance(v1, v2);
    cout << q[0];
    N++;
	return make_tuple(vector<int>(), vector<double>());
}