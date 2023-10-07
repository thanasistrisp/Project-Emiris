#include <vector>
#include <iterator>
#include <random>
// vector   is used for std::vector.
// iterator is used for std::back_insert_iterator, std::advance().
// random   is used for std::default_random_engine generator, std::normal_distribution, std::uniform_real_distribution and rand().

#include "lsh.h"

using std::vector;

// ---------- Public functions for class HashFunction ----------

HashFunction::HashFunction(int number_of_dimensions, int window)
: number_of_dimensions(number_of_dimensions), window(window)
{
    // t ~ U[0, w)
    std::default_random_engine generator;
    std::uniform_real_distribution<float> uniform(0.0, window);
    t = uniform(generator);

    // v ~ N(0, 1)^{d}
    std::normal_distribution<double> normal(0.0, 1.0);
    for(int i = 0; i < number_of_dimensions; i++){
        v.push_back(normal(generator));
    }
}

HashFunction::~HashFunction()
{

}

int HashFunction::hash(vector<double>& p)
{
    if(p.size() == 0){
        return -1;
    }

    // Use hash function h_i(p) = floor((p * v + t) / w)
    double result = std::inner_product(p.begin(), p.end(), v.begin(), t);
    return floor(result / window);
}

// todo: find a way to fix case p * v < 0

// ---------- Public functions for class HashFunctionFamily ----------

HashFunctionFamily::HashFunctionFamily(int number_of_dimensions, int number_of_hash_functions, int window)
: number_of_hash_functions(number_of_hash_functions)
{
    HashFunction *h;
    for(int i = 0; i < number_of_hash_functions; i++){
        h = new HashFunction(number_of_dimensions, window);
        hash_functions.push_back(h);
    }
}

HashFunctionFamily::~HashFunctionFamily()
{
    vector<HashFunction *>::const_iterator iter;
    for(iter = hash_functions.begin(); iter != hash_functions.end(); std::advance(iter, 1)){
        delete *iter;
    }
}

int HashFunctionFamily::hash(int i, vector<double>& p)
{
    return hash_functions.at(i)->hash(p);
}

// ---------- Public functions for class LSH ----------

LSH::LSH(int number_of_dimensions, int number_of_hash_functions, int window, int number_of_hash_tables, int number_of_buckets)
: number_of_dimensions(number_of_dimensions), number_of_hash_functions(number_of_hash_functions),
  number_of_hash_tables(number_of_hash_tables), number_of_buckets(number_of_buckets),
  hash_function_family(number_of_dimensions, number_of_hash_functions, window)
{
    // Calculate factors for hash function g.
    for(int j = 0; j < number_of_hash_tables; j++){
        std::vector<int> r;
        for(int i = 0; i < number_of_hash_functions; i++){
            r.push_back(rand());
        }
        factors.push_back(r);
    }
}

LSH::~LSH()
{

}

// void LSH::insert(vector<double>& p)
// {

// }

int LSH::hash(int j, vector<double>& p)
{
    if(j < 0 || j >= number_of_hash_tables){
        return -1;
    }

    // Use hash function g_j(p) = sum (r_i * h_i(p)) mod M
    int r_i, h_i, sum = 0;
    for(int i = 0; i < number_of_hash_functions; i++){
        r_i = factors.at(j).at(i);
        h_i = hash_function_family.hash(i, p);
        sum += r_i * h_i;
    }
    return sum % number_of_buckets;
}