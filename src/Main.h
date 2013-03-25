#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <queue>
#include <deque>
#include <iostream>
#include <sstream>

#include "Constants.h"
#include "Test.h"


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


void read_from_file(char*, struct Vertex[] , int&);
std::string deque_of_edges_to_string(std::deque<Edge>);
void franciosa_remove_all_pointing_to(std::deque<Edge>&, Vertex*);
Edge franciosa_edge_with_minimum_distance (std::deque<Edge>);
 
#endif
