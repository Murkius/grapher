#include "Main.h"

using namespace std;

/* ----------------- Breadth-first search (static, for non weighted graphs) ------------------- */
void bfs(Graph &g) {
    deque<struct Vertex*> Q; 
    struct Vertex* t, u;

    for(int vertexId = 0; vertexId < g.n; vertexId++){
        g.graph[vertexId].marked = false;
    }

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
    //TODO: ÃƒÆ’Ã¢â‚¬Å¾ÃƒÂ¢Ã¢â€šÂ¬Ã‚Â ÃƒÆ’Ã‚Â¢ÃƒÂ¢Ã¢â‚¬Å¡Ã‚Â¬Ãƒâ€¦Ã‚Â¾ÃƒÆ’Ã¢â‚¬Å¾Ãƒâ€šÃ‚Â¼ÃƒÆ’Ã†â€™Ãƒâ€šÃ‚Â¦ÃƒÆ’Ã¢â‚¬Å¡Ãƒâ€šÃ‚Â½ia x turÃƒÆ’Ã¢â‚¬Å¾ÃƒÂ¢Ã¢â€šÂ¬Ã‚Â ÃƒÆ’Ã‚Â¢ÃƒÂ¢Ã¢â‚¬Å¡Ã‚Â¬Ãƒâ€¦Ã‚Â¾ÃƒÆ’Ã¢â‚¬Å¾ÃƒÂ¯Ã‚Â¿Ã‚Â½ÃƒÆ’Ã‚Â¢ÃƒÂ¢Ã¢â€šÂ¬Ã…Â¡Ãƒâ€šÃ‚Â¬ÃƒÆ’Ã‚Â¢ÃƒÂ¢Ã¢â‚¬Å¡Ã‚Â¬ÃƒÂ¯Ã‚Â¿Ã‚Â½tÃƒÆ’Ã¢â‚¬Å¾ÃƒÂ¢Ã¢â€šÂ¬Ã‚Â ÃƒÆ’Ã‚Â¢ÃƒÂ¢Ã¢â‚¬Å¡Ã‚Â¬Ãƒâ€šÃ‚Â¦ÃƒÆ’Ã¢â‚¬Å¾ÃƒÂ¢Ã¢â‚¬Å¡Ã‚Â¬ÃƒÆ’Ã¢â‚¬Å¡Ãƒâ€šÃ‚Â³ bÃƒÆ’Ã¢â‚¬Å¾ÃƒÂ¢Ã¢â€šÂ¬Ã‚Â ÃƒÆ’Ã‚Â¢ÃƒÂ¢Ã¢â‚¬Å¡Ã‚Â¬Ãƒâ€šÃ‚Â¦ÃƒÆ’Ã¢â‚¬Å¾ÃƒÂ¢Ã¢â‚¬Å¡Ã‚Â¬ÃƒÆ’Ã¢â‚¬Å¡Ãƒâ€šÃ‚Â«ti ÃƒÆ’Ã¢â‚¬Å¾ÃƒÂ¢Ã¢â€šÂ¬Ã‚Â ÃƒÆ’Ã‚Â¢ÃƒÂ¢Ã¢â‚¬Å¡Ã‚Â¬Ãƒâ€¦Ã‚Â¾ÃƒÆ’Ã¢â‚¬Å¾ÃƒÂ¢Ã¢â€šÂ¬Ã‚Â ÃƒÆ’Ã‚Â¢ÃƒÂ¢Ã¢â‚¬Å¡Ã‚Â¬Ãƒâ€šÃ‚Â dedamas ÃƒÆ’Ã¢â‚¬Å¾ÃƒÂ¢Ã¢â€šÂ¬Ã‚Â ÃƒÆ’Ã‚Â¢ÃƒÂ¢Ã¢â‚¬Å¡Ã‚Â¬Ãƒâ€¦Ã‚Â¾ÃƒÆ’Ã¢â‚¬Å¾ÃƒÂ¢Ã¢â€šÂ¬Ã‚Â ÃƒÆ’Ã‚Â¢ÃƒÂ¢Ã¢â‚¬Å¡Ã‚Â¬Ãƒâ€šÃ‚Â  e.to.bs
    if((*e.to).distance_to_source > (*e.from).distance_to_source + 1) { //TODO: ÃƒÆ’Ã¢â‚¬Å¾ÃƒÂ¢Ã¢â€šÂ¬Ã‚Â ÃƒÆ’Ã‚Â¢ÃƒÂ¢Ã¢â‚¬Å¡Ã‚Â¬Ãƒâ€¦Ã‚Â¾ÃƒÆ’Ã¢â‚¬Å¾ÃƒÂ¢Ã¢â‚¬Å¡Ã‚Â¬ÃƒÆ’Ã†â€™Ãƒâ€¹Ã…â€œia turÃƒÆ’Ã¢â‚¬Å¾ÃƒÂ¢Ã¢â€šÂ¬Ã‚Â ÃƒÆ’Ã‚Â¢ÃƒÂ¢Ã¢â‚¬Å¡Ã‚Â¬Ãƒâ€¦Ã‚Â¾ÃƒÆ’Ã¢â‚¬Å¾ÃƒÂ¯Ã‚Â¿Ã‚Â½ÃƒÆ’Ã‚Â¢ÃƒÂ¢Ã¢â€šÂ¬Ã…Â¡Ãƒâ€šÃ‚Â¬ÃƒÆ’Ã‚Â¢ÃƒÂ¢Ã¢â‚¬Å¡Ã‚Â¬ÃƒÂ¯Ã‚Â¿Ã‚Â½tÃƒÆ’Ã¢â‚¬Å¾ÃƒÂ¢Ã¢â€šÂ¬Ã‚Â ÃƒÆ’Ã‚Â¢ÃƒÂ¢Ã¢â‚¬Å¡Ã‚Â¬Ãƒâ€šÃ‚Â¦ÃƒÆ’Ã¢â‚¬Å¾ÃƒÂ¢Ã¢â‚¬Å¡Ã‚Â¬ÃƒÆ’Ã¢â‚¬Å¡Ãƒâ€šÃ‚Â³ bÃƒÆ’Ã¢â‚¬Å¾ÃƒÂ¢Ã¢â€šÂ¬Ã‚Â ÃƒÆ’Ã‚Â¢ÃƒÂ¢Ã¢â‚¬Å¡Ã‚Â¬Ãƒâ€šÃ‚Â¦ÃƒÆ’Ã¢â‚¬Å¾ÃƒÂ¢Ã¢â‚¬Å¡Ã‚Â¬ÃƒÆ’Ã¢â‚¬Å¡Ãƒâ€šÃ‚Â«ti lyginama pagal rank
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
    Timer tx, tg;
    double time_sum;


    cout << "\nBfs1:\n";
    for(int k = 1; k <= gn; k++){
        tg.reset();
        tg.start();
        cout << k;
        //time_sum = 0;
        g = new Graph;
        (*g).readFromFile(numberedFilename("data/bfsMarked/experiment_graph_", k));
        (*g).readUpdatesFromFile(numberedFilename("data/bfsMarked/experiment_updates_", k), &updates);
        (*g).setSourceVertexByIndex(0);
        for(int r = 1; r <= repeats; r++){
            tx.reset();
            tx.start();

            bfs(*g);
            for(int updateId = 0; updateId < updates.size(); updateId++){
                (*updates[updateId].from).fs.push_back(updates[updateId].to);
                bfs(*g);
            }

            tx.stop();
            //time_sum += tx.getTime();
            cout << " " << tx.getTime();
            if(r != repeats){
                for(int updateId = 0; updateId < updates.size(); updateId++){
                    (*updates[updateId].from).fs.pop_back();
                }
            }
        }
        delete g;
        updates.clear();
        tg.stop();
        cout << " (" << tg.getTime() << ")\n";
    }

    cout << "\nBfs2:\n";
    for(int k = 1; k <= gn; k++){
        tg.reset();
        tg.start();
        cout << k;
        //time_sum = 0;
        g = new Graph;
        (*g).readFromFile(numberedFilename("data/bfsMarked/experiment_graph_", k));
        (*g).readUpdatesFromFile(numberedFilename("data/bfsMarked/experiment_updates_", k), &updates);
        (*g).setSourceVertexByIndex(0);
        for(int r = 1; r <= repeats; r++){
            tx.reset();
            tx.start();
            
            bfs2(*g);
            for(int updateId = 0; updateId < updates.size(); updateId++){
                (*updates[updateId].from).fs.push_back(updates[updateId].to);
                bfs2(*g);
            }
            
            tx.stop();
            //time_sum += tx.getTime();
            cout << " " << tx.getTime();
            if(r != repeats){
                for(int updateId = 0; updateId < updates.size(); updateId++){
                    (*updates[updateId].from).fs.pop_back();
                }
            }
        }
        delete g;
        updates.clear();
        tg.stop();
        cout << " (" << tg.getTime() << ")\n";
    }
    
    cout << "\nBfs3:\n";
    for(int k = 1; k <= gn; k++){
        tg.reset();
        tg.start();
        cout << k;
        //time_sum = 0;
        g = new Graph;
        (*g).readFromFile(numberedFilename("data/bfsMarked/experiment_graph_", k));
        (*g).readUpdatesFromFile(numberedFilename("data/bfsMarked/experiment_updates_", k), &updates);
        (*g).setSourceVertexByIndex(0);
        for(int r = 1; r <= repeats; r++){
            tx.reset();
            tx.start();
            
            bfs3(*g);
            for(int updateId = 0; updateId < updates.size(); updateId++){
                (*updates[updateId].from).fs.push_back(updates[updateId].to);
                bfs3(*g);
            }
            
            tx.stop();
            //time_sum += tx.getTime();
            cout << " " << tx.getTime();
            if(r != repeats){
                for(int updateId = 0; updateId < updates.size(); updateId++){
                    (*updates[updateId].from).fs.pop_back();
                }
            }
        }
        delete g;
        updates.clear();
        tg.stop();
        cout << " (" << tg.getTime() << ")\n";
    }
}

void scenario1(int gn) {
    Graph * g;
    std::vector<Edge> updates;
    double sbfs_time;

    /* ------------------------------ Warm up ------------------------------ */
    Timer tt;
    tt.start();
    g = new Graph;
    (*g).readFromFile(numberedFilename("data/bfsMarked/experiment_graph_", 11));
    (*g).setSourceVertexByIndex(0);
    (*g).readUpdatesFromFile(numberedFilename("data/bfsMarked/experiment_updates_", 11), &updates);
    bfs(*g);
    bfs(*g);
    delete g;
    updates.clear();
    tt.stop();
    /* ---------------------------- Warm up end ---------------------------- */

    Timer t1;
    for(int k = 1; k <= gn; k++){
        g = new Graph;
        (*g).readFromFile(numberedFilename("data/bfsMarked/experiment_graph_", k));
        (*g).readUpdatesFromFile(numberedFilename("data/bfsMarked/experiment_updates_", k), &updates);
        (*g).setSourceVertexByIndex(0);

        t1.reset();
        t1.start();
        bfs2(*g);
        t1.stop();
        sbfs_time = t1.getTime();
        cout << "FRA G:" << k << " " << sbfs_time;

        for(int updateId = 0; updateId < updates.size(); updateId++){
            t1.start();
            franciosa_insert(updates[updateId]);
            t1.stop();
            cout << " " << t1.getTime();
        }
        cout << "\n";

        delete g;
        updates.clear();
    }

    cout << "\n";

    Timer t2;
    for(int k = 1; k <= gn; k++){
        g = new Graph;
        (*g).readFromFile(numberedFilename("data/bfsMarked/experiment_graph_", k));
        (*g).readUpdatesFromFile(numberedFilename("data/bfsMarked/experiment_updates_", k), &updates);
        (*g).setSourceVertexByIndex(0);

        t2.reset();
        t2.start();
        bfs2(*g);
        t2.stop();
        sbfs_time = t2.getTime();
        cout << "BFS G:" << k << " " << sbfs_time;

        for(int updateId = 0; updateId < updates.size(); updateId++){
            t2.reset();
            t2.start();
            (*updates[updateId].from).fs.push_back(updates[updateId].to);
            for(int vertexId = 0; vertexId < (*g).n; vertexId++){
                (*g).graph[vertexId].marked = false;
            }
            bfs2(*g);
            t2.stop();
            cout << " " << (t2.getTime() + sbfs_time);
        }
        cout << "\n";

        delete g;
        updates.clear();
    }
}

int main() {
    //Test tester;
    //tester.run_tests();

    for(int i = 1; i <= 3; i++){
        cout << "\n --------------------------- Iteration " << i << " ---------------------------\n";
        scenario1(50);
    }

//    while(true){
//        cout << " --------- NEW ITERATION --------- " << time(0) << "\n";
//        testBfsVariants(50, 5);
//    }
    
    return 0;
}
