#include <vector>
#include <cmath>

using namespace std;

double euclidean_distance(const vector<double> &a, const vector<double> &b) {
	double result = 0;
	for (int i = 0; i < (int) a.size(); i++) {
		result += (a[i] - b[i]) * (a[i] - b[i]);
	}
	return sqrt(result);
}