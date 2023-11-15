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

        void add_edge(int, Vertex&);
        void add_edge(int, const std::vector<Vertex*>&);

        std::vector<Vertex*> get_successors(int) const;
};