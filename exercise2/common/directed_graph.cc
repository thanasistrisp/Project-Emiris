#include <vector>
#include <tuple>
#include <algorithm>

#include "directed_graph.hpp"

using namespace std;


DirectedGraph::DirectedGraph()
{

}

DirectedGraph::~DirectedGraph()
{
    for(int i = 0; i < (int) adjacency_lists.size(); i++){
        for(int j = 0; j < (int) adjacency_lists[i].size(); j++){
            delete adjacency_lists[i][j];
        }
    }
}

void DirectedGraph::add_vertex(int index)
{
    // If vertex already exists, skip.
    if(index < (int) adjacency_lists.size()){
        return;
    }
    vector<Vertex*> dummy;
    adjacency_lists.push_back(dummy);
}

void DirectedGraph::add_edge(int origin, int destination, double distance)
{
    if(origin >= (int) adjacency_lists.size()){
        return;
    }
    Vertex *vertex = new Vertex(destination, distance);
    adjacency_lists[origin].push_back(vertex);
}

void DirectedGraph::add_edge(int origin, const vector<int>& destinations, const vector<double>& distances)
{
    if(origin >= (int) adjacency_lists.size()){
        return;
    }
    if((int)destinations.size() != (int) distances.size()){
        return;
    }
    for(int i; i < (int) destinations.size(); i++){
        Vertex *vertex = new Vertex(destinations[i], distances[i]);
        adjacency_lists[origin].push_back(vertex);
    }
}

tuple<vector<int>, vector<double>> DirectedGraph::get_successors(int index) const
{
    if(index < 0 || index >= (int) adjacency_lists.size()){
        return make_tuple(vector<int>(), vector<double>());
    }
    vector<int> indices;
    vector<double> distances;
    for(int i = 0; i < (int) adjacency_lists[index].size(); i++){
        Vertex *v = adjacency_lists[index][i];
        indices.push_back(v->get_index());
        distances.push_back(v->get_distance());
    }
    return make_tuple(indices, distances);
}

tuple<vector<int>, vector<double>> DirectedGraph::get_successors(int index, int number_succ) const
{
    if(number_succ <= 0 || index < 0 || index >= (int) adjacency_lists.size()){
        return make_tuple(vector<int>(), vector<double>());
    }
    if(number_succ == (int) adjacency_lists[index].size()){
        return get_successors(index);
    }
    vector<int> indices;
    vector<double> distances;
    for(auto iter = adjacency_lists[index].begin(); ; iter++){
        if(iter == adjacency_lists[index].end() || indices.size() == number_succ){
            break;
        }
        Vertex *v = *iter;
        indices.push_back(v->get_index());
        distances.push_back(v->get_distance());
    }
    return make_tuple(indices, distances);
}

vector<int> DirectedGraph::get_predecessors(int index) const
{
    if(index < 0 || index >= (int) adjacency_lists.size()){
        return vector<int>();
    }
    vector<int> predecessors;
    for(int i = 0; i < (int) adjacency_lists.size(); i++){
        for(int j = 0; j < (int) adjacency_lists[i].size(); j++){
            if(adjacency_lists[i][j]->get_index() == index){
                predecessors.push_back(i);
            }
        }
    }
    return predecessors;
}

vector<int> DirectedGraph::get_predecessors(int index, int number_pred) const
{
    if(number_pred <= 0 || index < 0 || index >= (int) adjacency_lists.size()){
        return vector<int>();
    }
    vector<int> predecessors;
    for(int i = 0; i < (int) adjacency_lists.size(); i++){
        for(int j = 0; j < (int) adjacency_lists[i].size(); j++){
            if(adjacency_lists[i][j]->get_index() == index){
                predecessors.push_back(i);
            }
            if((int) predecessors.size() == number_pred){
                break;
            }
        }
    }
    return predecessors;
}