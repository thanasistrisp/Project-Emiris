#include "vertex.hpp"

Vertex::Vertex(int index, double distance = 0)
: index(index), distance(distance)
{

}

Vertex::~Vertex()
{

}

int Vertex::get_index() const
{
    return index;
}

double Vertex::get_distance() const
{
    return distance;
}