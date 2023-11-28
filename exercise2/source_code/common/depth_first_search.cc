#include <unordered_set>
#include <stack>

#include "directed_graph.hpp"
#include "depth_first_search.hpp"

using namespace std;

// tha exoume thema me ta labeling

// 

DirectedGraph* depth_first_search(const DirectedGraph &graph, int start_node)
{
    unordered_set<int> checked;
    vector<int> neighbors;
    stack<pair<int, int>> frontier;
    frontier.push(make_pair(-1, start_node));
    int parent, node;

    // Initialize DFS spanning tree (directed, because it has a specific root).
    DirectedGraph *spanning_tree = new DirectedGraph();
    for(int i = 0; i < (int) graph.get_number_of_nodes(); i++){
        spanning_tree->add_vertex(i);
    }

    while(!frontier.empty()){
        // Pop an edge (parent, node) from stack.
        tie(parent, node) = frontier.top();
        frontier.pop();

        // If node is unchecked.
        if(checked.find(node) == checked.end()){
            // Mark it as checked.
            checked.insert(node);

            // If node != root, add edge (parent, node) to the DFS spanning tree.
            if(parent != -1){
                spanning_tree->add_edge(parent, node);
            }
            
            // Extend frontier, add more edges to check later.
            neighbors = graph.get_successors(node);
            for(int i = 0; i < (int) neighbors.size(); i++){
                if(checked.find(neighbors[i]) == checked.end()){
                    frontier.push(make_pair(node, neighbors[i]));
                }
            }
        }
    }
    return spanning_tree;
}