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
    std::vector<Vertex*> fs;
};
struct Edge {
    Vertex* from;
    Vertex* to;
    char operation;
};

class Graph {
    public:
        int n; //Number of vertices
        struct Vertex* graph;
        
        Graph();
        ~Graph();
        
        void generate(std::string, float, float);
        void generateInsertions(int, std::vector<Edge>*);
        
        void readFromFile(std::string);
        void saveToFile(std::string);
        void readUpdatesFromFile(std::string, std::vector<Edge>*);
        void saveUpdatesToFile(std::string, std::vector<Edge>);
        
        void setSourceVertexByIndex(int);
        struct Vertex* getSourceVertex();
        
        std::string spTreeToString();
    private:
        struct Vertex *sourceVertex;
        
        bool reachableFromSource();
        void generateByGnp(int, float);
};

#endif
