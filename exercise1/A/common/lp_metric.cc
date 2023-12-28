#include <vector>
#include <iterator>
#include <string>
#include <cmath>
// iterator  is used for std::const_iterator, std::advance().
// cmath     is used for fabs(), pow().

#include "lp_metric.hpp"

using std::vector;
using std::string;

double euclidean_distance(const std::vector<double>& v1, const std::vector<double>& v2)
{
    if(v1.size() != v2.size() || v1.size() == 0){
        return -1;
    }

    double sum = 0.0;
    for(int i = 0; i < (int) v1.size(); i++){
        sum += pow(fabs(v1.at(i) - v2.at(i)), 2);
    }
    return sqrt(sum);
}

double lp_metric(vector<double>& v1, vector<double>& v2, int p = 2)
{
    if(p < 0 || v1.size() != v2.size() || v1.size() == 0){
        return -1;
    }

    double sum = 0.0;
    for(int i = 0; i < (int) v1.size(); i++){
        sum += pow(fabs(v1.at(i) - v2.at(i)), p);
    }
    return pow(sum, 1 / p);
}

double lp_metric(vector<double>& v, string p)
{
    if(v.size() == 0 || p.compare("inf") != 0){
        return -1;
    }
    double max = v.at(0);
    vector<double>::const_iterator iter;
    for(iter = v.begin(); iter != v.end(); std::advance(iter, 1)){
        if(*iter > max){
            max = *iter;
        }
    }
    return max;
}