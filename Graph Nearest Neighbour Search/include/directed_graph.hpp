#pragma once

#include <vector>
#include <fstream>

class DirectedGraph
{
    private:
        std::vector<std::vector<int>> adjacency_lists;

    public:
        DirectedGraph();
        ~DirectedGraph();

        // Adds a new vertex with the given index.
        void add_vertex(int);

        // Adds an edge with origin and destination with the given indices.
        void add_edge(int, int);
        // Adds multiple edges with the same origin.
        void add_edge(int, const std::vector<int>&);

        // Returns the total number of nodes in the graph.
        unsigned int get_number_of_nodes() const;

        // Returns (all or the first k) successors of the node with the given index.
        std::vector<int> get_successors(int) const;
        std::vector<int> get_successors(int, int) const;

        // Returns (all or the first k) predecessors of the node with the given index.
        std::vector<int> get_predecessors(int) const;
        std::vector<int> get_predecessors(int, int) const;

        // Saves/loads the graph to/from a .bin file.
        void save(std::ofstream&) const;
        void load(std::ifstream&);
};