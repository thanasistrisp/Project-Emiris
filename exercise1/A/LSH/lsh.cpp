#include <vector>
#include <iterator>
#include <random>
// vector   is used for std::vector.
// iterator is used for std::back_insert_iterator, std::advance().
// random   is used for std::default_random_engine generator, std::normal_distribution, std::uniform_real_distribution and rand().

#include "lsh.h"
#include "list.h"
#include "hash_table.h"

using std::vector;

// ---------- Public functions for class LSH ----------

LSH::LSH(int number_of_dimensions, int number_of_points, int number_of_hash_functions, int number_of_hash_tables, int window)
: number_of_dimensions(number_of_dimensions), number_of_hash_functions(number_of_hash_functions),
  table_size(number_of_points / 4), number_of_hash_tables(number_of_hash_tables)
{
    hash_tables = new HashTable<vector<double>, int>*[number_of_hash_tables];
    for(int i = 0; i < number_of_hash_tables; i++){
        hash_tables[i] = new HashTable<vector<double>, int>(table_size, number_of_dimensions, number_of_hash_functions, window);
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