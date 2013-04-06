#ifndef GRAPH_H
#define GRAPH_H

#include <cstdlib>
#include <iostream>
#include <ctime>
#include <string>
#include <sstream>

#include "Constants.h"

struct Vertex {
    bool marked;
    Vertex* parent;
    int identifier;
    int distance_to_source;
    int fs_size;
    Vertex* fs[max_vertices];
    
    int childNumber;
    Vertex* children[max_vertices];
};
struct Edge {
    Vertex* from;
    Vertex* to;
};

class Graph {
    public:
        int n; //Number of vertices
        struct Vertex graph[max_vertices];
        
        Graph::Graph();
        
        void generateByGnp(int, float);
        
        void readFromFile(const char[]);
        
        void setSourceVertexByIndex(int);
        struct Vertex* getSourceVertex();
        
        std::string spTreeToString();
    private:
        struct Vertex *sourceVertex;
        
};

#endif
