#include <vector>

#include "directed_graph.hpp"

using namespace std;


DirectedGraph::DirectedGraph()
{

}

DirectedGraph::~DirectedGraph()
{
    for(int i = 0; i < adjacency_lists.size(); i++){
        for(int j = 0; j < adjacency_lists[i].size(); j++){
            delete adjacency_lists[i][j];
        }
    }
}

void DirectedGraph::add_vertex(int index)
{
    // If vertex already exists, skip.
    if(index < adjacency_lists.size()){
        return;
    }
    vector<Vertex*> dummy;
    adjacency_lists.push_back(dummy);
}

void DirectedGraph::add_edge(int origin, int destination, double distance)
{
    if(origin == adjacency_lists.size()){
        add_vertex(origin);
    }
    Vertex *vertex = new Vertex(destination, distance);
    adjacency_lists[origin].push_back(vertex);
}

void DirectedGraph::add_edge(int origin, const vector<int>& destinations, const vector<double>& distances)
{
    if(origin == adjacency_lists.size()){
        add_vertex(origin);
    }
    if(destinations.size() != distances.size()){
        return;
    }
    for(int i; i < destinations.size(); i++){
        if(destinations[i] >= adjacency_lists.size()){
            add_vertex(destinations[i]);
        }
        Vertex *vertex = new Vertex(destinations[i], distances[i]);
        adjacency_lists[origin].push_back(vertex);
    }
}

vector<Vertex*> DirectedGraph::get_successors(int index) const
{
    if(index < 0 || index >= adjacency_lists.size()){
        return vector<Vertex*>();
    }
    return adjacency_lists[index];
}

vector<Vertex*> DirectedGraph::get_successors(int index, int number_succ) const
{
    if(number_succ <= 0 || index < 0 || index >= adjacency_lists.size()){
        return vector<Vertex*>();
    }
    if(number_succ == adjacency_lists[index].size()){
        return get_successors(index);
    }
    return vector<Vertex*>(adjacency_lists[index].begin(), adjacency_lists[index].begin() + number_succ);
}