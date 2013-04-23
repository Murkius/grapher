#include "Main.h"

using namespace std;

    const int numberOfGraphs = 30;
    const int n = 3000; //30 graphs * 100 times
    Graph graphArray[n];
    std::vector<Edge> updates[n];

/* ----------------- Breadth-first search (static, for non weighted graphs) ------------------- */
void bfs(Graph &g) {
    deque<struct Vertex*> Q; 
    struct Vertex* t, u;
    Q.push_back(g.getSourceVertex());
    (*g.getSourceVertex()).marked = true;
    while (not Q.empty()) {
        t = Q.front();
        Q.pop_front();
        for (int c = 0; c < (*t).fs_size; c++) {
            if (not (*(*t).fs[c]).marked) {
                (*(*t).fs[c]).marked = true;
                (*(*t).fs[c]).parent = t;
                (*(*t).fs[c]).distance_to_source = (*t).distance_to_source+1;
                Q.push_back((*t).fs[c]);
            }
        }
    }
}

void bfs2(Graph &g) {
    bool marks[max_vertices] = {};
    queue<struct Vertex*> Q; 
    struct Vertex* t, u;
    
    Q.push(g.getSourceVertex());
    //(*g.getSourceVertex()).marked = true;
    marks[(*g.getSourceVertex()).identifier] = true;
    while (not Q.empty()) {
        t = Q.front();
        Q.pop();
        for (int c = 0; c < (*t).fs_size; c++) {
            if (not marks[(*(*t).fs[c]).identifier]) {
                marks[(*(*t).fs[c]).identifier] = true;
                (*(*t).fs[c]).parent = t;
                (*(*t).fs[c]).distance_to_source = (*t).distance_to_source+1;
                Q.push((*t).fs[c]);
            }
        }
    }
}

void bfs3(Graph &g) {
    bool markSign = not (*g.getSourceVertex()).marked;
    queue<struct Vertex*> Q; 
    struct Vertex* t, u;
    
    Q.push(g.getSourceVertex());
    (*g.getSourceVertex()).marked = markSign;
    while (not Q.empty()) {
        t = Q.front();
        Q.pop();
        for (int c = 0; c < (*t).fs_size; c++) {
            if ((*(*t).fs[c]).marked != markSign) {
                (*(*t).fs[c]).marked = markSign;
                (*(*t).fs[c]).parent = t;
                (*(*t).fs[c]).distance_to_source = (*t).distance_to_source+1;
                Q.push((*t).fs[c]);
            }
        }
    }
}
/* ------------------------------ Breadth-first search end------------------------------------- */

/* ---------------------- Franciosa (dynamic, for non weighted graphs) ------------------------ */
Edge franciosa_edge_with_minimum_distance (deque<Edge> d) {
    deque<Edge>::iterator i = d.begin();
    int min_distance = std::numeric_limits<int>::max();
    Edge min_distance_edge;
    
    while(i != d.end()) {
        if((*(*i).from).distance_to_source < min_distance) {
            min_distance = (*(*i).from).distance_to_source;
            min_distance_edge = *i;
        }
        i++;
    }
    return min_distance_edge;
}
void franciosa_remove_all_pointing_to(deque<Edge> &d, Vertex *v) {
    deque<Edge>::iterator i = d.begin();
    while(not d.empty() && i != d.end()) {
        if((*i).to == v) {
            d.erase(i);
        }
        i++;
    }
}
void franciosa_propagate(deque<Edge> d) {
    while(!d.empty()) {
        Edge min_edge = franciosa_edge_with_minimum_distance(d);
        franciosa_remove_all_pointing_to(d, min_edge.to);
        (*min_edge.to).parent = min_edge.from;
        
        if((*min_edge.to).distance_to_source > (*min_edge.from).distance_to_source + 1){
            (*min_edge.to).distance_to_source = (*min_edge.from).distance_to_source + 1;
            for(int i = 0; i < (*min_edge.to).fs_size; i++){
                if((*(*min_edge.to).fs[i]).distance_to_source > (*min_edge.to).distance_to_source +1){//TODO: lyginimas pagal rank
                    Edge e;
                    e.from = min_edge.to;
                    e.to = (*min_edge.to).fs[i];
                    d.push_front(e);
                }
            }
        }
    }
}
void franciosa_insert(Edge e) {
    (*e.from).fs[(*e.from).fs_size++] = e.to;
    //TODO: Čia x turėtų būti įdedamas į e.to.bs
    if((*e.to).distance_to_source > (*e.from).distance_to_source + 1) { //TODO: čia turėtų būti lyginama pagal rank
        deque<Edge> d;
        d.push_front(e);
        franciosa_propagate(d);
    }
}
void franciosa_delete() {

}
/* ------------------------------------- Franciosa end ---------------------------------------- */

string deque_of_edges_to_string(deque<Edge> d) {
    deque<Edge>::iterator i = d.begin();
    ostringstream content;
    
    while(i != d.end()) {
        content << "From " <<(*(*i).from).identifier << " to " 
                << (*(*i).to).identifier << "\n";
        i++;
    }
    return content.str();
}
std::string numberedFilename(std::string prefix, int number){
    std::ostringstream strStream;
    strStream << prefix << number;
    return strStream.str();
}

void prepareGraphsForBFSTesting(){
    cout << "Preloading graphs & updates start\n";
    for(int i = 0; i < n; i++){
        graphArray[i].readFromFile(numberedFilename("data/bfsMarked/experiment_graph_", i % numberOfGraphs+1).c_str());
        graphArray[i].setSourceVertexByIndex(0);
        graphArray[i].readUpdatesFromFile(numberedFilename("data/bfsMarked/experiment_updates_", i % numberOfGraphs+1).c_str(), &updates[i]);
    }
    cout << "Preloading graphs & updates end\n";
}
void testBfsVariants(){
    prepareGraphsForBFSTesting();

    Timer t1;
    t1.start();  
    for(int k = 0; k < n; k++){
        
        bfs(graphArray[k]);
        for(int updateId = 0; updateId < updates[k].size(); updateId++){
            for(int vertexId = 0; vertexId < graphArray[k].n; vertexId++){
                graphArray[k].graph[vertexId].marked = false;
            }
            (*updates[k][updateId].from).fs[(*updates[k][updateId].from).fs_size++] = updates[k][updateId].to;
            bfs(graphArray[k]);
        }
    }
    t1.stop();
    cout << "Bfs1: " << t1.getTime() << " " << t1.getClockTime() << "\n";
    
    prepareGraphsForBFSTesting();
    
    Timer t2;
    t2.start();  
    for(int k = 0; k < n; k++){
        
        bfs2(graphArray[k]);
        for(int updateId = 0; updateId < updates[k].size(); updateId++){
            (*updates[k][updateId].from).fs[(*updates[k][updateId].from).fs_size++] = updates[k][updateId].to;
            bfs2(graphArray[k]);
        }
    }
    t2.stop();
    cout << "Bfs2: " << t2.getTime() << " " << t2.getClockTime() << "\n";
    
    prepareGraphsForBFSTesting();
    
    Timer t3;
    t3.start();  
    for(int k = 0; k < n; k++){
        
        bfs3(graphArray[k]);
        for(int updateId = 0; updateId < updates[k].size(); updateId++){
            (*updates[k][updateId].from).fs[(*updates[k][updateId].from).fs_size++] = updates[k][updateId].to;
            bfs3(graphArray[k]);
        }
    }
    t3.stop();
    cout << "Bfs3: " << t3.getTime() << " " << t3.getClockTime() << "\n";
}

int main() {
    //Test tester;
    //tester.run_tests();

    testBfsVariants();
    
    return 0;
}
