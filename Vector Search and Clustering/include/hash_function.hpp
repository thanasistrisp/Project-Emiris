#pragma once

#include <vector>

// Hash function in Euclidean space.
class HashFunction
{
    private:
        int number_of_dimensions; // Number of dimensions d.
        double window;               // Window w for the entire algorithm.
        float t;                  // Shift t.
        std::vector<double> v;    // d-dimensional vector with coordinates in N(0, 1).

    public:
        // Initializes a hash function with the given number of dimensions and window.
        HashFunction(int, double);
        ~HashFunction();

        // Returns the hashed value of the given vector.
        int hash(const std::vector<double>&);
};