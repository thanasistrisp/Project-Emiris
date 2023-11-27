#include <vector>
#include <tuple>
#include <unordered_set>
#include <set>

#include "generic_search.hpp"
#include "directed_graph.hpp"
#include "set_utils.hpp"

#include <iostream>

using namespace std;

tuple<tuple<vector<int>, vector<double>>,
      tuple<vector<int>, vector<double>>> generic_search_on_graph(const DirectedGraph &graph, const vector<vector<double>>& dataset,
                                                                  int start_node, const vector<double>& query, int total_candidates, unsigned int k,
                                                                  double (*distance)(const vector<double>&, const vector<double>&))
{
    // Candidate set R = \emptyset.
    multiset<pair<int, double>*, decltype(&set_cmp)> candidates(&set_cmp);
    unordered_set<int> unique_indices;
    unordered_set<int> checked_indices;
    vector<pair<int, double>*> checked_pairs;
    vector<int> neighbors;

    // R.add(p), i = 1.
    pair<int, double>* p = new pair(start_node, distance(dataset[start_node], query));
    candidates.insert(p);
    unique_indices.insert(start_node);

    // While i < L.
    while((int) candidates.size() < total_candidates){
        // p = the first unchecked node in R.
        for(auto iter = candidates.begin(); iter != candidates.end(); iter++){
            p = *iter;
            if(checked_indices.find(p->first) == checked_indices.end()){
                break;
            }
        }
        // Check if we have gone through all candidates
        // (avoid infinite loop when L candidates can't be found).
        if(checked_indices.find(p->first) != checked_indices.end()){
            break;
        }
        // Mark p as checked.
        checked_indices.insert(p->first);
        checked_pairs.push_back(p);
        
        // For every neighbor N of p \in G : N \not \in R
        //  R.add(N)
        //  i++
        // Sort R in ascending order of the distance to q.
        neighbors = graph.get_successors(p->first);

        for(int i = 0; i < (int) neighbors.size(); i++){
            if(unique_indices.find(neighbors[i]) != unique_indices.end()){
                continue;
            }
            p = new pair(neighbors[i], distance(dataset[neighbors[i]], query));
            candidates.insert(p);
            unique_indices.insert(neighbors[i]);
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

    // Convert set of checked pairs to two vectors.
    vector<int> checked_indices_vector;
    vector<double> checked_distances;
    for(auto& it : checked_pairs){
        checked_indices_vector.push_back(it->first);
        checked_distances.push_back(it->second);
    }

    for(auto iter = candidates.begin(); iter != candidates.end(); iter++){
        delete *iter;
    }

    return make_tuple(make_tuple(indices, distances), make_tuple(checked_indices_vector, checked_distances));
}