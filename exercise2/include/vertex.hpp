#pragma once

class Vertex
{
    private:
        int index;
        double distance;

    public:
        Vertex(int, double);
        ~Vertex();

        int get_index() const;
        double get_distance() const;
};