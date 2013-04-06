#include "Main.h"

using namespace std;

/* ----------------- Breadth-first search (static, for non weighted graphs) ------------------- */
void bfs(Graph g) {
    queue<struct Vertex*> Q; 
    struct Vertex* t, u;
    
    Q.push(g.getSourceVertex());
    (*g.getSourceVertex()).marked = true;
    while (not Q.empty()) {
        t = Q.front();
        Q.pop();
        for (int c = 0; c < (*t).fs_size; c++) {
            if (not (*(*t).fs[c]).marked) {
                (*(*t).fs[c]).marked = true;
                (*(*t).fs[c]).parent = t;
                (*(*t).fs[c]).distance_to_source = (*t).distance_to_source+1;
                (*t).children[(*t).childNumber++] = (*t).fs[c];
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
        
        //TODO: pašalinti iš ankstesnio tėvo vaikų sąrašo (tiksli pozicija nežinoma) 
        // ir visus paskiau einančius vaikus perkelti 1 vieta atgal - ėda per daug laiko
        (*min_edge.from).children[(*min_edge.from).childNumber++] = min_edge.to;
        
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

int main() {
    Test t;
    t.run_tests();

    Graph g;
    g.readFromFile("data/graph1.txt");
    g.setSourceVertexByIndex(0);
    
    bfs(g);
    cout << g.spTreeToString(); 

    Edge e;
    e.from = &g.graph[0];
    e.to = &g.graph[3];
    franciosa_insert(e);

    cout << "\nPo briaunos 1 -> 4 pridejimo:\n";
    cout << g.spTreeToString(); 

    return 0;
}
