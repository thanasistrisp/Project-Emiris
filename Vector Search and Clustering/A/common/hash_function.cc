#include <vector>
#include <iterator>
#include <random>
#include <ctime>
#include <chrono>
// vector   is used for std::vector.
// iterator is used for std::back_insert_iterator, std::advance().
// random   is used for std::random_device, std::default_random_engine generator, std::normal_distribution, std::uniform_real_distribution and rand().

#include "hash_function.hpp"

using std::vector;

// ---------- Functions for class HashFunction ---------- //

HashFunction::HashFunction(int number_of_dimensions, double window)
: number_of_dimensions(number_of_dimensions), window(window)
{
    // t ~ U[0, w)
    std::random_device rd;
    std::default_random_engine random_engine(rd());
    std::uniform_real_distribution<float> uniform(0.0, window);
    t = uniform(random_engine);

    // v ~ N(0, 1)^{d}
    std::normal_distribution<double> normal(0.0, 1.0);
    for(int i = 0; i < number_of_dimensions; i++){
        v.push_back(normal(random_engine));
    }
}

HashFunction::~HashFunction()
{

}

int HashFunction::hash(const vector<double>& p)
{
    if(p.size() == 0){
        return -1;
    }

    // Use hash function h_i(p) = floor((p * v + t) / w)
    double result = std::inner_product(p.begin(), p.end(), v.begin(), t);
    return floor(fabs(result) / window);
}