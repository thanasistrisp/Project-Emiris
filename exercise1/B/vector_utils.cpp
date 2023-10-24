#include <vector>

#include "vector_utils.h"

using namespace std;

vector<double> vector_addition(const vector<double>& v1, const vector<double>& v2){
    if(v1.size() != v2.size()){
        return v1;
    }
    vector<double> result;
    for(int i = 0; i < (int) v1.size(); i++){
        result.push_back(v1[i] + v2[i]);
    }
    return result;
}

vector<double> vector_subtraction(const vector<double>& v1, const vector<double>& v2){
    if(v1.size() != v2.size()){
        return v1;
    }
    vector<double> result;
    for(int i = 0; i < (int) v2.size(); i++){
        result.push_back(- v2[i]);
    }
    return vector_addition(v1, result);
}

vector<double> vector_scalar_mult(const vector<double>& v, double n){
    vector<double> result;
    for(int i = 0; i < (int) v.size(); i++){
        result.push_back(v[i] * n);
    }
    return result;
}