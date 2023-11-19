#include <vector>
#include <tuple>
#include <unordered_set>
#include <set>

#include "generic_search.hpp"
#include "directed_graph.hpp"
#include "set_utils.hpp"

using namespace std;

tuple<vector<int>, vector<double>> generic_search_on_graph(const DirectedGraph &graph, const vector<vector<double>>& dataset,
                                                           int start_node, const vector<double>& query, int total_candidates, unsigned int k,
                                                           double (*distance)(const vector<double>&, const vector<double>&))
{
    // Candidate set R = \emptyset.
    multiset<pair<int, double>*, decltype(&set_cmp)> candidates(&set_cmp);
    unordered_set<int> checked_nodes;
    vector<int> neighbors;

    // R.add(p), i = 1.
    // int p = start_node;
    pair<int, double>* p = new pair(start_node, distance(dataset[start_node], query));
    candidates.insert(p);

    // While i < L.
    while((int) candidates.size() < total_candidates){
        // p = the first unchecked node in R.
        for(auto iter = candidates.begin(); iter != candidates.end(); iter++){
            p = *iter;
            if(checked_nodes.find(p->first) == checked_nodes.end()){
                break;;
            }
        }
        // Mark p as checked.
        checked_nodes.insert(p->first);
        
        // For every neighbor N of p \in G : N \not \in R
        //  R.add(N)
        //  i++
        // Sort R in ascending order of the distance to q.
        neighbors = graph.get_successors(p->first);
        for(int i = 0; i < (int) neighbors.size(); i++){
            p = new pair(neighbors[i], 0.0);
            if(candidates.find(p) != candidates.end()){
                delete p;
                continue;
            }
            p->second = distance(dataset[neighbors[i]], query);
            candidates.insert(p);
        }
    }
    vector<int> indices;
    vector<double> distances;
    for(auto iter = candidates.begin(); iter != candidates.end(); iter++){
        p = *iter;
        indices.push_back(p->first);
        distances.push_back(p->second);
        if(indices.size() == k){
            break;
        }
    }
    return make_tuple(indices, distances);
}