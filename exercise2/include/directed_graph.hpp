#pragma once

#include <vector>

class DirectedGraph
{
    private:
        std::vector<std::vector<int>> adjacency_lists;

    public:
        DirectedGraph();
        ~DirectedGraph();

        void add_vertex(int);

        void add_edge(int, int);
        void add_edge(int, const std::vector<int>&);

        std::vector<int> get_successors(int) const;
        std::vector<int> get_successors(int, int) const;

        std::vector<int> get_predecessors(int) const;
        std::vector<int> get_predecessors(int, int) const;
};