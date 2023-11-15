#include <vector>

#include "directed_graph.hpp"

using namespace std;


DirectedGraph::DirectedGraph()
{

}

DirectedGraph::~DirectedGraph()
{

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

void DirectedGraph::add_edge(int origin, Vertex& destination)
{
    if(origin == adjacency_lists.size()){
        add_vertex(origin);
    }
    adjacency_lists[origin].push_back(&destination);
}

void DirectedGraph::add_edge(int origin, const vector<Vertex*>& destinations)
{
    if(origin == adjacency_lists.size()){
        add_vertex(origin);
    }
    if(adjacency_lists[origin].size() == 0){
        adjacency_lists[origin] = destinations;
        return;
    }
    for(int i; i < destinations.size(); i++){
        if(destinations[i]->get_index() >= adjacency_lists.size()){
            add_vertex(destinations[i]->get_index());
        }
        adjacency_lists[origin].push_back(destinations[i]);
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
    if(number_succ <= 0 | index < 0 || index >= adjacency_lists.size()){
        return vector<Vertex*>();
    }
    if(number_succ == adjacency_lists[index].size()){
        return get_successors(index);
    }
    return vector<Vertex*>(adjacency_lists[index].begin(), adjacency_lists[index].begin() + number_succ);
}