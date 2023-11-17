#pragma once

#include <vector>

#include "vertex.hpp"

class DirectedGraph
{
    private:
        std::vector<std::vector<Vertex*>> adjacency_lists; 

    public:
        DirectedGraph();
        ~DirectedGraph();

        void add_vertex(int);

        void add_edge(int, int, double);
        void add_edge(int, const std::vector<int>&, const std::vector<double>&);

        std::vector<Vertex*> get_successors(int) const;
        std::vector<Vertex*> get_successors(int, int) const;

        std::vector<int> get_predecessors(int) const;
        std::vector<int> get_predecessors(int, int) const;
};