#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <ctime>
#include <string>
#include <sstream>
#include <queue>
#include <cmath>

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
    char operation;
};

class Graph {
    public:
        int n; //Number of vertices
        struct Vertex graph[max_vertices];
        
        Graph();
        
        void generate(std::string, float, float);
        void generateInsertions(int, std::vector<Edge>*);
        
        void readFromFile(const char[]);
        void saveToFile(const char[]);
        void readUpdatesFromFile(const char[], std::vector<Edge>*);
        void saveUpdatesToFile(const char[], std::vector<Edge>);
        
        void setSourceVertexByIndex(int);
        struct Vertex* getSourceVertex();
        
        std::string spTreeToString();
    private:
        struct Vertex *sourceVertex;
        
        bool reachableFromSource();
        void generateByGnp(int, float);
};

#endif
