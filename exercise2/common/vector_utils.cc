#include <vector>

#include "vector_utils.hpp"

using namespace std;

void insertion_sort(vector<double>& v, double d)
{
    v.push_back(d);
    for(int i = v.size() - 1; i > 0; i--){
        if(v[i] < v[i - 1]){
            swap(v[i], v[i - 1]);
        }
        else{
            break;
        }
    }
}