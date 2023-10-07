#include <vector>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <string>
#include <cmath>
// vector    is used for std::vector.
// algorithm is used for std::transform().
// iterator  is used for std::back_insert_iterator, std::advance().
// numeric   is used for std::accumulate().
// cmath     is used for abs(), pow().
// string    is used for std::string.

#include "lp_metric.h"

using std::vector;
using std::string;


double lp_metric(vector<double>& v1, vector<double>& v2, int p = 2)
{
    if(p < 0 || v1.size() != v2.size()){
        return -1;
    }

    double sum = 0.0;
    for(int i = 0; i < (int) v1.size(); i++){
        sum += pow(abs(v1.at(i) - v2.at(i)), p);
    }
    return pow(sum, 1 / p);
}

double lp_metric(vector<double>& v, string p)
{
    if(p.compare("inf") != 0){
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