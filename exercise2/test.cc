#include <iostream>
#include <vector>

#include "directed_graph.hpp"

using namespace std;

int main()
{
    DirectedGraph g;
    g.add_vertex(0);
    g.add_vertex(1);
    g.add_vertex(2);
    g.add_edge(0, 1, 3.0);
    g.add_edge(0, 2, 1.2);
    g.add_edge(0, 3, 0.2);
    g.add_edge(1, 0, 2);
    g.add_edge(1, 2, 0.3);
    cout << "Sucessors of vertex with index 0" << endl;
    vector<Vertex*> s = g.get_successors(0, 2);
    for(int i = 0; i < s.size(); i++){
        cout << s[i]->get_index() << " " << s[i]->get_distance() << endl;
    }
    cout << "Sucessors of vertex with index 1" << endl;
    s = g.get_successors(1);
    for(int i = 0; i < s.size(); i++){
        cout << s[i]->get_index() << " " << s[i]->get_distance() << endl;
    }
    cout << "Sucessors of vertex with index 2" << endl;
    s = g.get_successors(2);
    for(int i = 0; i < s.size(); i++){
        cout << s[i]->get_index() << " " << s[i]->get_distance() << endl;
    }
    cout << "Sucessors of vertex with index 3" << endl;
    s = g.get_successors(3);
    for(int i = 0; i < s.size(); i++){
        cout << s[i]->get_index() << " " << s[i]->get_distance() << endl;
    }
    return 0;
}