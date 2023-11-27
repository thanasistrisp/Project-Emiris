#include <unordered_set>
#include <stack>

#include "directed_graph.hpp"
#include "depth_first_search.hpp"

using namespace std;

unordered_set<int> depth_first_search(const DirectedGraph &graph, int start_node)
{
    unordered_set<int> checked;
    vector<int> neighbors;
    stack<int> frontier;
    frontier.push(start_node);
    int node;

    while(!frontier.empty()){
        node = frontier.top();
        frontier.pop();

        if(checked.find(node) == checked.end()){
            checked.insert(node);
            
            neighbors = graph.get_successors(node);
            for(int i = 0; i < (int) neighbors.size(); i++){
                if(checked.find(neighbors[i]) == checked.end()){
                    frontier.push(neighbors[i]);
                }
            }
        }
    }
    return checked;
}