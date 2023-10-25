#pragma once

#include <vector>

// Hash function in Euclidean space.
class HashFunction
{
    private:
        int number_of_dimensions; // Number of dimensions d.
        int window;               // Window w for the entire algorithm.
        float t;                  // Shift t.
        std::vector<double> v;    // d-dimensional vector with coordinates in N(0, 1).

    public:
        HashFunction(int, int);
        ~HashFunction();

        int hash(const std::vector<double>&);
};