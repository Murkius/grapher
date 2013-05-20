#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <queue>
#include <deque>
#include <iostream>
#include <limits>
#include <sstream>
#include <fstream>
#include <algorithm>

#include "Test.h"
#include "Graph.h"
#include "Timer.h"

std::string deque_of_edges_to_string(std::deque<Edge>);
std::string spTreeToString_1(struct Vertex, int);
std::string spTreeToString_2(struct Vertex[], int);

void bfs(Graph &);

void franciosa_remove_all_pointing_to(std::deque<Edge>&, Vertex*);
Edge franciosa_edge_with_minimum_distance (std::deque<Edge>);
void franciosa_insert(Edge);
 
#endif
