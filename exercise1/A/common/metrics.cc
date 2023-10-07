#include <vector>
#include <cmath>

using namespace std;

double euclidean_distance(vector<double> a, vector<double> b) {
	double result = 0;
	for (int i = 0; i < (int) a.size(); i++) {
		result += (a[i] - b[i]) * (a[i] - b[i]);
	}
	return sqrt(result);
}