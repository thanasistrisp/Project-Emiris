#include <vector>
#include <tuple>
#include <algorithm>
#include <fstream>

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
    if(index < (int) adjacency_lists.size()){
        return;
    }
    vector<int> dummy;
    adjacency_lists.push_back(dummy);
}

void DirectedGraph::add_edge(int origin, int destination)
{
    if(origin >= (int) adjacency_lists.size()){
        return;
    }
    adjacency_lists[origin].push_back(destination);
}

void DirectedGraph::add_edge(int origin, const vector<int>& destinations)
{
    if(origin >= (int) adjacency_lists.size() || destinations.size() == 0){
        return;
    }
    if(adjacency_lists[origin].size() == 0){
        adjacency_lists[origin] = destinations;
        return;
    }
    for(int i = 0; i < (int) destinations.size(); i++){
        adjacency_lists[origin].push_back(destinations[i]);
    }
}

vector<int> DirectedGraph::get_successors(int index) const
{
    if(index < 0 || index >= (int) adjacency_lists.size()){
        return vector<int>();
    }
    return adjacency_lists[index];
}

vector<int> DirectedGraph::get_successors(int index, int number_succ) const
{
    if(number_succ <= 0 || index < 0 || index >= (int) adjacency_lists.size()){
        return vector<int>();
    }
    if(number_succ == (int) adjacency_lists[index].size()){
        return get_successors(index);
    }
    return vector<int>(adjacency_lists[index].begin(), adjacency_lists[index].begin() + number_succ);
}

vector<int> DirectedGraph::get_predecessors(int index) const
{
    if(index < 0 || index >= (int) adjacency_lists.size()){
        return vector<int>();
    }
    vector<int> predecessors;
    for(int i = 0; i < (int) adjacency_lists.size(); i++){
        for(int j = 0; j < (int) adjacency_lists[i].size(); j++){
            if(adjacency_lists[i][j] == index){
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
            if(adjacency_lists[i][j] == index){
                predecessors.push_back(i);
            }
            if((int) predecessors.size() == number_pred){
                break;
            }
        }
    }
    return predecessors;
}

void DirectedGraph::save(ofstream& file) const
{
    int size = adjacency_lists.size();
    file.write((char*) &size, sizeof(int));
    for(int i = 0; i < size; i++){
        int list_size = adjacency_lists[i].size();
        file.write((char*) &list_size, sizeof(int));
        for(int j = 0; j < list_size; j++){
            file.write((char*) &adjacency_lists[i][j], sizeof(int));
        }
    }
}

void DirectedGraph::load(ifstream& file)
{
    int size;
    file.read((char*) &size, sizeof(int));
    adjacency_lists.resize(size);
    for(int i = 0; i < size; i++){
        int list_size;
        file.read((char*) &list_size, sizeof(int));
        adjacency_lists[i].resize(list_size);
        for(int j = 0; j < list_size; j++){
            file.read((char*) &adjacency_lists[i][j], sizeof(int));
        }
    }
}