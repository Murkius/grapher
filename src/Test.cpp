#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "Test.h"
#include "Main.h"
#include "Constants.h"

using namespace std;

void Test::run_tests(){
    cout << "Begin testing\n";
    cout << " - Graph reading from file\t\t\t";
    Test::graphReadingFromFile();
    cout << "OK\n - Deque of edges ToString\t\t\t";
    Test::dequeOfEdgesToString();
    cout << "OK\n - Breath first search (1)\t\t\t";
    Test::bfs_1();
    cout << "OK\n - Breath first search (2)\t\t\t";
    Test::bfs_2();
    cout << "OK\n - Franciosa: Remove edges pointing to (1)\t";
    Test::franciosaRemoveEdgesPointingTo_1();
    cout << "OK\n - Franciosa: Remove edges pointing to (2)\t";
    Test::franciosaRemoveEdgesPointingTo_2();
    cout << "OK\n - Franciosa: Edge with minimum distance\t";
    Test::franciosaEdgeWithMinimumDistance();
    cout << "OK\n - Franciosa: Edge insert (1)\t\t\t";
    Test::franciosaEdgeInsert_1();
    cout << "OK\n";
    cout << "Testing ended successfully\n\n\n";
}
void Test::graphReadingFromFile(){
    struct Vertex graph[max_vertices];
    int n;
    
    read_from_file("data/graph1.txt", graph, n);
    assert(n==4);
    assert(graph[1].identifier==2);
    assert(graph[1].fs_size==2);
    assert((*graph[1].fs[0]).identifier==3);
    assert((*graph[1].fs[1]).identifier==4);
}
void Test::dequeOfEdgesToString(){
    struct Vertex graph[max_vertices];
    int n;
    
    read_from_file("data/graph1.txt", graph, n);
    deque<Edge> d;
    Edge e1, e2;
    e1.from = &graph[0];
    e1.to = &graph[1];
    e2.from = &graph[1];
    e2.to = &graph[3];
    d.push_front(e1);
    d.push_front(e2);
    string str = "From 2 to 4\nFrom 1 to 2\n";
    assert(str == deque_of_edges_to_string(d));
}
void Test::bfs_1(){
    struct Vertex graph[max_vertices];
    int n;

    read_from_file("data/graph1.txt", graph, n);
    bfs(graph, &graph[0]);
    string s = "1\n  2\n    3\n    4\n";
    assert(s == spTreeToString_2(graph, n)); 
}
void Test::bfs_2(){
    struct Vertex graph[max_vertices];
    int n;

    read_from_file("data/graph2.txt", graph, n);
    bfs(graph, &graph[0]);
    string s = "1\n  2\n    5\n      6\n        9\n  4\n    7\n    8\n      3\n";
    assert(s == spTreeToString_2(graph, n)); 
}
void Test::franciosaRemoveEdgesPointingTo_1(){
    struct Vertex graph[max_vertices];
    int n;
    
    read_from_file("data/graph1.txt", graph, n);
    deque<Edge> d;
    Edge e1, e2, e3;
    e1.from = &graph[0];
    e1.to = &graph[1];
    e2.from = &graph[1];
    e2.to = &graph[3];
    e3.from = &graph[2];
    e3.to = &graph[3];
    d.push_front(e1);
    d.push_front(e2);
    d.push_front(e3);
    franciosa_remove_all_pointing_to(d, &graph[3]);
    string str = "From 1 to 2\n";
    assert(str == deque_of_edges_to_string(d));
}
void Test::franciosaRemoveEdgesPointingTo_2(){
    struct Vertex graph[max_vertices];
    int n;
    
    read_from_file("data/graph1.txt", graph, n);
    deque<Edge> d;
    Edge e1, e2, e3;
    e2.from = &graph[1];
    e2.to = &graph[3];
    e3.from = &graph[2];
    e3.to = &graph[3];
    d.push_front(e2);
    d.push_front(e3);
    franciosa_remove_all_pointing_to(d, &graph[3]);
    assert(d.empty());
}
void Test::franciosaEdgeWithMinimumDistance(){
    struct Vertex graph[max_vertices];
    int n;
    
    read_from_file("data/graph1.txt", graph, n);
    graph[0].distance_to_source = 0;
    graph[1].distance_to_source = 1;
    graph[2].distance_to_source = 2;
    graph[3].distance_to_source = 2;
    deque<Edge> d;
    Edge e1, e2, e3;
    e1.from = &graph[1];
    e1.to = &graph[3];
    e2.from = &graph[2];
    e2.to = &graph[3];
    e3.from = &graph[2];
    e3.to = &graph[3];
    d.push_front(e1);
    d.push_front(e2);
    d.push_front(e3);
    Edge returnedEdge = franciosa_edge_with_minimum_distance(d);
    assert(e1.from == returnedEdge.from);
    assert(e1.to == returnedEdge.to);
}
void Test::franciosaEdgeInsert_1(){
    struct Vertex graph[max_vertices];
    int n;

    read_from_file("data/graph1.txt", graph, n);

    bfs(graph, &graph[0]);
    
    Edge e;
    e.from = &graph[0];
    e.to = &graph[3];
    franciosa_insert(e);
    
    string s = "1\n  2\n    3\n  4\n";
    assert(s == spTreeToString_2(graph, n)); 
}

