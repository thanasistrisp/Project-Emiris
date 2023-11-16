#include <vector>
#include <iterator>
#include <tuple>
#include <set>
// iterator is used for std::const_iterator, std::advance().

#include "lsh.hpp"
#include "list.hpp"
#include "hash_table.hpp"

using std::vector;
using std::tuple;
using std::get;
using std::make_tuple;
using std::set;
using std::prev;

// ---------- Functions for class LSH ---------- //

// Initializes an instance with the given number of hash functions,
// number of hash tables, table size and window.
// The last argument is the set of points the LSH algorithm will be applied to.
LSH::LSH(int number_of_hash_functions, int number_of_hash_tables, int table_size, int window, const vector<vector<double>> &dataset)
: number_of_dimensions(dataset.at(0).size()), number_of_hash_functions(number_of_hash_functions),
  table_size(table_size), number_of_hash_tables(number_of_hash_tables), dataset(dataset)
{
    hash_tables = new HashTable<vector<double>, int>*[number_of_hash_tables];
    for(int i = 0; i < number_of_hash_tables; i++){
        hash_tables[i] = new HashTable<vector<double>, int>(table_size, number_of_dimensions, number_of_hash_functions, window);
    }

    // Insert data to all hash tables.
    for(int i = 0; (unsigned int) i < dataset.size(); i++){
        insert(dataset.at(i), i);
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

// Inserts the given data point with the given index to all L hash tables. 
void LSH::insert(vector<double> p, int index)
{
    for(int i = 0; i < number_of_hash_tables; i++){
        hash_tables[i]->insert(p, index);
    }
}

// Returns the indices of the k-approximate nearest neighbours (ANN) of the given query q
// and their distances to the query based on the given distance function.
// Last parameter indicates whether or not the Querying trick is applied.
tuple<vector<int>, vector<double>> LSH::query(const vector<double>& q, unsigned int k,
                                              double (*distance)(const vector<double>&, const vector<double>&),
                                              bool querying_trick)
{
    auto compare = [](tuple<int, double> t1, tuple<int, double> t2){ return get<1>(t1) < get<1>(t2); };
    set<tuple<int, double>, decltype(compare)> s(compare);

    double dist;
    int p_index;
    bool valid = true;
    // unsigned int q_secondary_key;

    for(int i = 0; i < number_of_hash_tables; i++){

        q_secondary_key = hash_tables[i]->secondary_hash_function(q);

        while((p_index = hash_tables[i]->get_data(q, valid)) != 0 || valid){
            vector<double> p = dataset.at(p_index);

            // Skip query if found.
            if(p == q){
                continue;
            }

            // Choose only the points that share the same ID inside the bucket (Querying trick).
            if(querying_trick && hash_tables[i]->secondary_hash_function(p) != q_secondary_key){
                continue;
            }

            // Keep k items only to save space.
            dist = distance(p, q);
            if(s.size() == k){
                if(dist >= get<1>(*s.rbegin())){
                    continue;
                }
                s.erase(std::prev(s.end()));
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

// Returns the indices of the k-approximate nearest neighbours (ANN) of the given query q
// and their distances to the query based on the given distance function.
// All the neighbours returned lie within radius r.
tuple<vector<int>, vector<double>> LSH::query_range(const vector<double>& q, double r,
                                                    double (*distance)(const vector<double>&, const vector<double>&))
{
    auto compare = [](tuple<int, double> t1, tuple<int, double> t2){ return get<1>(t1) < get<1>(t2); };
    set<tuple<int, double>, decltype(compare)> s(compare);

    double dist;
    int p_index;
    bool valid = true;
    for(int i = 0; i < number_of_hash_tables; i++){
        while((p_index = hash_tables[i]->get_data(q, valid)) != 0 || valid){
            vector<double> p = dataset.at(p_index);
            dist = distance(p, q);
            if(dist < r){
                s.insert(make_tuple(p_index, dist));
            }
            // if(s.size() > (unsigned int) 20 * number_of_hash_tables){ // Optional.
            //     break;
            // }
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