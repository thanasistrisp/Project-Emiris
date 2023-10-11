#include <vector>
#include <iterator>
#include <random>
// vector   is used for std::vector.
// iterator is used for std::back_insert_iterator, std::advance().
// random   is used for std::default_random_engine generator, std::normal_distribution, std::uniform_real_distribution and rand().

#include "lsh.h"
#include "list.h"
#include "hash_table.h"

#include <tuple>
#include <set>

using std::vector;
using std::tuple;
using std::get;
using std::make_tuple;
using std::set;

// ---------- Public functions for class LSH ----------

LSH::LSH(int number_of_dimensions, int number_of_points, int number_of_hash_functions,
         int number_of_hash_tables, int window, vector<vector<double>> *dataset)
: number_of_dimensions(number_of_dimensions), number_of_hash_functions(number_of_hash_functions),
  table_size(number_of_points / 4), number_of_hash_tables(number_of_hash_tables), dataset(dataset)
{
    hash_tables = new HashTable<vector<double>, int>*[number_of_hash_tables];
    for(int i = 0; i < number_of_hash_tables; i++){
        hash_tables[i] = new HashTable<vector<double>, int>(table_size, number_of_dimensions, number_of_hash_functions, window);
    }

    // Insert data to all hash tables.
    for(int i = 0; (unsigned int) i < dataset->size(); i++){
        insert(dataset->at(i), i);
    }
}

LSH::~LSH()
{
    for(int i = 0; i < number_of_hash_tables; i++){
        if(hash_tables[i] != NULL){
            delete hash_tables[i];
        }
    }
    delete[] hash_tables;
}

void LSH::insert(vector<double> p, int index)
{
    for(int i = 0; i < number_of_hash_tables; i++){
        hash_tables[i]->insert(p, index);
    }
}

tuple<vector<int>, vector<double>> LSH::query(const vector<double>& q, unsigned int k,
                                              double (*distance)(const vector<double>&, const vector<double>&))
{
    auto compare = [](tuple<int, double> t1, tuple<int, double> t2){ return get<1>(t1) <= get<1>(t2); };
    set<tuple<int, double>, decltype(compare)> s(compare);

    double dist;
    int p_index;
    for(int i = 0; i < number_of_hash_tables; i++){
        while((p_index = hash_tables[i]->get_data(q)) != 0){
            vector<double> p = dataset->at(p_index);
            dist = distance(p, q);
            if(s.size() == k){
                if(dist < get<1>(*s.begin())){
                    s.erase(s.begin());
                }
                continue;
            }
            s.insert(make_tuple(p_index, dist));
        }
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

tuple<vector<int>, vector<double>> LSH::query_range(const vector<double>& q, double r,
                                               double (*distance)(const vector<double>&, const vector<double>&))
{
    double dist;
    int p_index;
    vector<int> indices;
    vector<double> distances;
    for(int i = 0; i < number_of_hash_tables; i++){
        while((p_index = hash_tables[i]->get_data(q)) != 0){
            vector<double> p = dataset->at(p_index);
            dist = distance(p, q);
            if(dist < r){
                indices.push_back(p_index);
                distances.push_back(dist);
            }
            if(indices.size() > (unsigned int) 20 * number_of_hash_tables){
                return make_tuple(indices, distances);
            }
        }
    }
    return make_tuple(indices, distances);
}