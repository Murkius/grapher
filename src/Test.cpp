#include "Test.h"

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
    cout << "OK\n - Franciosa: Edge insert (2)\t\t\t";
    Test::franciosaEdgeInsert_2();
    cout << "OK\n - Franciosa: Edge insert (3)\t\t\t";
    Test::franciosaEdgeInsert_3();
    cout << "OK\n";
    cout << "Testing ended successfully\n\n\n";
}
void Test::graphReadingFromFile(){
    Graph g;
    
    g.readFromFile("data/graph1.txt");
    assert(g.n==4);
    assert(g.graph[1].identifier==2);
    assert(g.graph[1].fs_size==2);
    assert((*g.graph[1].fs[0]).identifier==3);
    assert((*g.graph[1].fs[1]).identifier==4);
}
void Test::dequeOfEdgesToString(){
    Graph g;
    
    g.readFromFile("data/graph1.txt");
    deque<Edge> d;
    Edge e1, e2;
    e1.from = &g.graph[0];
    e1.to = &g.graph[1];
    e2.from = &g.graph[1];
    e2.to = &g.graph[3];
    d.push_front(e1);
    d.push_front(e2);
    string str = "From 2 to 4\nFrom 1 to 2\n";
    assert(str == deque_of_edges_to_string(d));
}
void Test::bfs_1(){
    Graph g;

    g.readFromFile("data/graph1.txt");
    g.setSourceVertexByIndex(0);
    
    bfs(g);
    
    string s = "1\n  2\n    3\n    4\n";
    assert(s == g.spTreeToString()); 
}
void Test::bfs_2(){
    Graph g;

    g.readFromFile("data/graph2.txt");
    g.setSourceVertexByIndex(0);
    
    bfs(g);
    
    string s = "1\n  2\n    5\n      6\n        9\n  4\n    7\n    8\n      3\n";
    assert(s == g.spTreeToString()); 
}
void Test::franciosaRemoveEdgesPointingTo_1(){
    Graph g;
    
    g.readFromFile("data/graph1.txt");
    deque<Edge> d;
    Edge e1, e2, e3;
    e1.from = &g.graph[0];
    e1.to = &g.graph[1];
    e2.from = &g.graph[1];
    e2.to = &g.graph[3];
    e3.from = &g.graph[2];
    e3.to = &g.graph[3];
    d.push_front(e1);
    d.push_front(e2);
    d.push_front(e3);
    franciosa_remove_all_pointing_to(d, &g.graph[3]);
    string str = "From 1 to 2\n";
    assert(str == deque_of_edges_to_string(d));
}
void Test::franciosaRemoveEdgesPointingTo_2(){
    Graph g;
    
    g.readFromFile("data/graph1.txt");
    deque<Edge> d;
    Edge e1, e2, e3;
    e2.from = &g.graph[1];
    e2.to = &g.graph[3];
    e3.from = &g.graph[2];
    e3.to = &g.graph[3];
    d.push_front(e2);
    d.push_front(e3);
    franciosa_remove_all_pointing_to(d, &g.graph[3]);
    assert(d.empty());
}
void Test::franciosaEdgeWithMinimumDistance(){
    Graph g;
    
    g.readFromFile("data/graph1.txt");
    g.graph[0].distance_to_source = 0;
    g.graph[1].distance_to_source = 1;
    g.graph[2].distance_to_source = 2;
    g.graph[3].distance_to_source = 2;
    deque<Edge> d;
    Edge e1, e2, e3;
    e1.from = &g.graph[1];
    e1.to = &g.graph[3];
    e2.from = &g.graph[2];
    e2.to = &g.graph[3];
    e3.from = &g.graph[2];
    e3.to = &g.graph[3];
    d.push_front(e1);
    d.push_front(e2);
    d.push_front(e3);
    Edge returnedEdge = franciosa_edge_with_minimum_distance(d);
    assert(e1.from == returnedEdge.from);
    assert(e1.to == returnedEdge.to);
}
void Test::franciosaEdgeInsert_1(){
    Graph g;

    g.readFromFile("data/graph1.txt");
    g.setSourceVertexByIndex(0);
    
    bfs(g);
    
    Edge e;
    e.from = &g.graph[0];
    e.to = &g.graph[3];
    franciosa_insert(e);
    
    string s = "1\n  2\n    3\n  4\n";
    assert(s == g.spTreeToString()); 
}
void Test::franciosaEdgeInsert_2(){
    Graph g;

    g.readFromFile("data/graph2.txt");
    g.setSourceVertexByIndex(0);
    
    bfs(g);
    
    Edge e;
    e.from = &g.graph[0];
    e.to = &g.graph[5];
    franciosa_insert(e);
    
    string s = "1\n  2\n    5\n  4\n    7\n    8\n      3\n  6\n    9\n";
    assert(s == g.spTreeToString()); 
}
void Test::franciosaEdgeInsert_3(){
    Graph g;

    g.readFromFile("data/graph2.txt");
    g.setSourceVertexByIndex(0);
    
    bfs(g);
    
    Edge e;
    e.from = &g.graph[1];
    e.to = &g.graph[8];
    franciosa_insert(e);

    string s = "1\n  2\n    5\n      6\n    9\n  4\n    7\n    8\n      3\n";
    assert(s == g.spTreeToString()); 
}

