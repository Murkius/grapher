#include "Main.h"

using namespace std;

/* ----------------- Breadth-first search (static, for non weighted graphs) ------------------- */
void bfs(Graph &g) {
    deque<struct Vertex*> Q; 
    struct Vertex* t, u;
    Q.push_back(g.getSourceVertex());
    (*g.getSourceVertex()).marked = true;
    while (not Q.empty()) {
        t = Q.front();
        Q.pop_front();
        for (int c = 0; c < (*t).fs.size(); c++) {
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
    bool marks [10000] = {};
    //std::fill_n(marks, g.n, false);
    
    queue<struct Vertex*> Q; 
    struct Vertex* t, u;

    Q.push(g.getSourceVertex());
    //(*g.getSourceVertex()).marked = true;
    marks[(*g.getSourceVertex()).identifier] = true;
    while (not Q.empty()) {
        t = Q.front();
        Q.pop();
        for (int c = 0; c < (*t).fs.size(); c++) {
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
        for (int c = 0; c < (*t).fs.size(); c++) {
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
            for(int i = 0; i < (*min_edge.to).fs.size(); i++){
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
    (*e.from).fs.push_back(e.to);
    //TODO: Ä†â€žÄ¼Ã¦Â½ia x turÄ†â€žÄ�â‚¬â€�tÄ†â€¦Ä€Â³ bÄ†â€¦Ä€Â«ti Ä†â€žÄ†â€ dedamas Ä†â€žÄ†â€  e.to.bs
    if((*e.to).distance_to_source > (*e.from).distance_to_source + 1) { //TODO: Ä†â€žÄ€Ã˜ia turÄ†â€žÄ�â‚¬â€�tÄ†â€¦Ä€Â³ bÄ†â€¦Ä€Â«ti lyginama pagal rank
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

void testBfsVariants(int gn, int repeats){
    Graph *g;
    std::vector<Edge> updates;
    Timer tx;

    Timer t0;
    t0.start();  
    for(int r = 1; r <= repeats; r++){
        cout << "FP R:" << r << "  ";
        for(int k = 1; k <= gn; k++){
            tx.reset();
            tx.start();
            g = new Graph;
            (*g).readFromFile(numberedFilename("data/bfsMarked/experiment_graph_", k));
            (*g).readUpdatesFromFile(numberedFilename("data/bfsMarked/experiment_updates_", k), &updates);
            /* In this place we would do bfs */
            delete g;
            updates.clear();
            tx.stop();
            cout << " " << k << ":" << tx.getTime();
        }
        cout << "\n";
    }
    t0.stop();
    //cout << "GrafÅ³ paruoÅ¡imas vienam kartui: " << t0.getTime() << " " << t0.getClockTime() << "\n";


    Timer t1;
    t1.start();
    for(int r = 1; r <= repeats; r++){
        cout << "Bfs1 R:" << r << "  ";
        for(int k = 1; k <= gn; k++){
            tx.reset();
            tx.start();
            g = new Graph;
            (*g).readFromFile(numberedFilename("data/bfsMarked/experiment_graph_", k));
            (*g).readUpdatesFromFile(numberedFilename("data/bfsMarked/experiment_updates_", k), &updates);

            (*g).setSourceVertexByIndex(0);
            bfs(*g);
            for(int updateId = 0; updateId < updates.size(); updateId++){
                for(int vertexId = 0; vertexId < (*g).n; vertexId++){
                    (*g).graph[vertexId].marked = false;
                }
                (*updates[updateId].from).fs.push_back(updates[updateId].to);
                bfs(*g);
            }

            delete g;
            updates.clear();
            tx.stop();
            cout << " " << k << ":" << tx.getTime();
        }
        cout << "\n";
    }
    t1.stop();
    //cout << "Bfs1 viso: " << t1.getTime() << " " << t1.getClockTime() << "\n";
    //cout << "Bfs1: " << (t1.getTime()-t0.getTime()) << "\n";

    Timer t2;
    t2.start();
    for(int r = 1; r <= repeats; r++){
        cout << "Bfs2 R:" << r << "  ";
        for(int k = 1; k <= gn; k++){
            tx.reset();
            tx.start();
            g = new Graph;
            (*g).readFromFile(numberedFilename("data/bfsMarked/experiment_graph_", k));
            (*g).readUpdatesFromFile(numberedFilename("data/bfsMarked/experiment_updates_", k), &updates);
            (*g).setSourceVertexByIndex(0);

            bfs2(*g);
            for(int updateId = 0; updateId < updates.size(); updateId++){
                (*updates[updateId].from).fs.push_back(updates[updateId].to);
                bfs2(*g);
            }

            delete g;
            updates.clear();
            tx.stop();
            cout << " " << k << ":" << tx.getTime();
        }
        cout << "\n";
    }
    t2.stop();
    //cout << "Bfs2 viso: " << t2.getTime() << " " << t2.getClockTime() << "\n";
    //cout << "Bfs2: " << (t2.getTime()-t0.getTime()) << "\n";

    Timer t3;
    t3.start();
    for(int r = 1; r <= repeats; r++){
        cout << "Bfs3 R:" << r << "  ";
        for(int k = 1; k <= gn; k++){
            tx.reset();
            tx.start();
            g = new Graph;
            (*g).readFromFile(numberedFilename("data/bfsMarked/experiment_graph_", k));
            (*g).readUpdatesFromFile(numberedFilename("data/bfsMarked/experiment_updates_", k), &updates);
            (*g).setSourceVertexByIndex(0);

            bfs3(*g);
            for(int updateId = 0; updateId < updates.size(); updateId++){
                (*updates[updateId].from).fs.push_back(updates[updateId].to);
                bfs3(*g);
            }

            delete g;
            updates.clear();
            tx.stop();
            cout << " " << k << ":" << tx.getTime();
        }
        cout << "\n";
    }
    t3.stop();
    //cout << "Bfs3 viso: " << t3.getTime() << " " << t3.getClockTime() << "\n";
    //cout << "Bfs3: " << (t3.getTime()-t0.getTime()) << "\n";

    cout << t0.getTime() << " | " << (t1.getTime()-t0.getTime()) << "  " << (t2.getTime()-t0.getTime()) << "  " << (t3.getTime()-t0.getTime()) << " " << time(0) << "\n";
}

int main() {
    //Test tester;
    //tester.run_tests();

    cout << time(0) << "\n";
    cout << "Prep    | Bfs1     Bfs2     Bfs3\n";
    while(true){
        cout << " --------- NEW ITERATION --------- \n";
        testBfsVariants(50, 5);
    }
    return 0;
}
