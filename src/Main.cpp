#include "Main.h"

using namespace std;

int n; //Number of vertices in a graph
struct Vertex graph[max_vertices];

void read_from_file(char* filename, struct Vertex graph[], int& n) {
    FILE *fp;
    int t;

    fp = fopen(filename,"r"); // read mode
    if( fp == NULL ) {
        perror("Error while opening the file.\n");
        exit(EXIT_FAILURE);
    }

    fscanf(fp, "%d", &n);
    
    for (int c = 0; c < n; c++) {
        graph[c].marked = false;
        graph[c].parent = 0;
        graph[c].childNumber = 0;
        fscanf(fp, "%d %d", &(graph[c].identifier), &(graph[c].fs_size));
        for(int i = 0; i < graph[c].fs_size; i++) {
            fscanf(fp, "%d", &t);
            graph[c].fs[i] = &(graph[t-1]); //TODO: pataisyti
        }
    }

    fclose(fp);
}

/* ----------------- Breadth-first search (static, for non weighted graphs) ------------------- */
void bfs(struct Vertex graph[], struct Vertex *source) {
    queue<struct Vertex*> Q; 
    struct Vertex* t, u;
    
    Q.push(source);
    (*source).marked = true;
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
    int min_distance = (*(*i).from).distance_to_source;
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
    while(i != d.end()) {
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
    (*e.from).fs_size++;
    (*e.from).fs[(*e.from).fs_size] = e.to;
    cout << "Èia x turëtø bûti ádedama á y.bs\n"; //TODO:
    if((*e.to).distance_to_source > (*e.from).distance_to_source + 1) { //TODO: èia turëtø bûti lyginama pagal rank
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
string spTreeToString(struct Vertex source, int identLevel) {
    ostringstream content;
    for(int i = 0; i < identLevel; i++) {
        content << " ";
    }
    content << source.identifier << "\n";
    for(int c = 0; c < source.childNumber; c++) {
        content << spTreeToString((*source.children[c]), identLevel+2);
    }
    return content.str();
}
int main() {
    Test t;
    t.run_tests();
    
    read_from_file("data/graph2.txt", graph, n);

    printf("Vertex number: %d\n", n);
    bfs(graph, &graph[0]);
    cout << spTreeToString(graph[0], 0);
    return 0;
    for(int i = 0; i < n; i++) {
        cout << "Vertice: " << graph[i].identifier << "\n  Childs: ";
        for(int j = 0; j < graph[i].fs_size; j++) {
            cout << (*graph[i].fs[j]).identifier << " ";
        }
        cout << "\n  Parent: ";
        if(graph[i].parent != 0) {
            cout << (*graph[i].parent).identifier;
        }
        else {
            cout << "No parent";
        }
        cout << "\n  Distance: " << graph[i].distance_to_source << "\n";
        cout << "  Child number: " << graph[i].childNumber << "\n";
    }
    return 0;
}
