#include "Main.h"

using namespace std;
int fra_prop_called;
int bfs_prop_called;

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
                bfs_prop_called++;
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
Edge franciosa_edge_with_minimum_distance (std::vector<Edge> d) {
    std::vector<Edge>::iterator i = d.begin();
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
void franciosa_remove_all_pointing_to(std::vector<Edge> &d, Vertex *v) {
    std::vector<Edge>::iterator i = d.begin();
    while(not d.empty() && i != d.end()) {
        if((*i).to == v) {
            d.erase(i);
        }
        else{
            i++;
        }
    }
}
void franciosa_propagate(std::vector<Edge> d) {
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
                    d.push_back(e);
                }
            }
        }
    }
}
void franciosa_insert(Edge e) {
    (*e.from).fs.push_back(e.to);
    if((*e.to).distance_to_source > (*e.from).distance_to_source + 1) {
        fra_prop_called++;
        std::vector<Edge> d;
        d.push_back(e);
        franciosa_propagate(d);
    }
}
void franciosa_optimised_insert(Edge e, std::list<Edge> &ru){
    bool inserted = false;
    (*e.from).fs.push_back(e.to);
    for (std::list<Edge>::iterator it = ru.begin(); it != ru.end(); ++it){
        if(it->from->distance_to_source >= e.from->distance_to_source){
            ru.insert(it, e);
            inserted = true;
            break;
        }
    }
    if(!inserted){
        ru.push_back(e);
    }
}
void franciosa_optimised_flush(std::list<Edge> ru){
    for (std::list<Edge>::iterator it = ru.begin(); it != ru.end(); ++it){
        if(it->to->distance_to_source > it->from->distance_to_source + 1) {
            fra_prop_called++;
            std::vector<Edge> d;
            d.push_back(*it);
            franciosa_propagate(d);
        }
    }
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
std::string numberedAndVersionedFilename(std::string prefix, int version, int number){
    std::ostringstream strStream;
    strStream << prefix << version << "_" << number;
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

void scenario1(int gn, int updates_version) {
    Graph * g;
    std::vector<Edge> updates;

    ofstream cout_t("scenario1_times.txt");
    ofstream cout_p("scenario1_propagates.txt");
    
    double sbfs_time;

    /* ------------------------------ Warm up ------------------------------ */
    Timer tt;
    tt.start();
    g = new Graph;
    (*g).readFromFile(numberedFilename("data/bfsMarked/experiment_graph_", 11));
    (*g).setSourceVertexByIndex(0);
    (*g).readUpdatesFromFile(numberedAndVersionedFilename("data/bfsMarked/experiment_updates_", 1, 11), &updates);
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
        (*g).readUpdatesFromFile(numberedAndVersionedFilename("data/bfsMarked/experiment_updates_", updates_version, k), &updates);
        for(int svi = 0; svi < (*g).n; svi++){
            (*g).setSourceVertexByIndex(svi);

            t1.reset();
            t1.start();
            bfs2(*g);
            t1.stop();
            sbfs_time = t1.getTime();
            cout_t << "FRA G:" << k << " SVI:" << svi << " " << sbfs_time;
            cout_p << "FRA G:" << k << " SVI:" << svi;

            fra_prop_called = 0;
            for(int updateId = 0; updateId < updates.size(); updateId++){
                t1.start();
                franciosa_insert(updates[updateId]);
                t1.stop();
                cout_t << " " << t1.getTime();
                cout_p << " " << fra_prop_called;
            }
            cout_t << "\n";
            cout_p << "\n";
            //Grafo atitaisymas
            if(svi != (*g).n-1){
                for(int updateId = 0; updateId < updates.size(); updateId++){
                    (*updates[updateId].from).fs.pop_back();
                }
            }
        }
        delete g;
        updates.clear();
    }

    cout_t << "\n";
    cout_p << "\n";

    Timer t2;
    for(int k = 1; k <= gn; k++){
        g = new Graph;
        (*g).readFromFile(numberedFilename("data/bfsMarked/experiment_graph_", k));
        (*g).readUpdatesFromFile(numberedAndVersionedFilename("data/bfsMarked/experiment_updates_", updates_version, k), &updates);
        for(int svi = 0; svi < (*g).n; svi++){
            (*g).setSourceVertexByIndex(svi);

            t2.reset();
            t2.start();
            bfs2(*g);
            t2.stop();
            sbfs_time = t2.getTime();
            cout_t << "BFS G:" << k << " SVI:" << svi << " " << sbfs_time;
            cout_p << "BFS G:" << k << " SVI:" << svi;

            (*updates[0].from).fs.push_back(updates[0].to);
            t2.reset();
            t2.start();
            bfs_prop_called = 0;
            bfs2(*g);
            t2.stop();
            cout_t << " " << (t2.getTime() + sbfs_time);
            cout_p << " " << bfs_prop_called;
            for(int updateId = 1; updateId < updates.size(); updateId++){
                (*updates[updateId].from).fs.push_back(updates[updateId].to);                
            }
            t2.reset();
            t2.start();
            bfs_prop_called = 0;
            bfs2(*g);
            t2.stop();
            cout_t << " " << (t2.getTime() + sbfs_time) << "\n";
            cout_p << " " << bfs_prop_called << "\n";

            //Grafo atitaisymas
            if(svi != (*g).n-1){
                for(int updateId = 0; updateId < updates.size(); updateId++){
                    (*updates[updateId].from).fs.pop_back();
                }
            }
        }
        delete g;
        updates.clear();
    }

    cout_t << "\n";
    cout_p << "\n";

    Timer t3;
    std::list<Edge> ru;
    for(int k = 1; k <= gn; k++){
        g = new Graph;
        (*g).readFromFile(numberedFilename("data/bfsMarked/experiment_graph_", k));
        (*g).readUpdatesFromFile(numberedAndVersionedFilename("data/bfsMarked/experiment_updates_", updates_version, k), &updates);
        for(int svi = 0; svi < (*g).n; svi++){
            (*g).setSourceVertexByIndex(svi);

            t3.reset();
            t3.start();
            bfs2(*g);
            t3.stop();
            sbfs_time = t3.getTime();
            cout_t << "FRO G:" << k << " SVI:" << svi << " " << sbfs_time;
            cout_p << "FRO G:" << k << " SVI:" << svi;

            for(int updateId = 0; updateId < updates.size(); updateId++){
                fra_prop_called = 0;
                t3.start();
                ru.clear();
                for(int i = 0; i <= updateId; i++){
                    franciosa_optimised_insert(updates[i], ru);
                }
                franciosa_optimised_flush(ru);
                t3.stop();

                //Grafo atitaisymas
                for(int i = 0; i <= updateId; i++){
                    (*updates[i].from).fs.pop_back();
                }
                bfs2(*g);

                cout_t << " " << t3.getTime();
                cout_p << " " << fra_prop_called;
            }
            cout_t << "\n";
            cout_p << "\n";
            
        }
        delete g;
        updates.clear();
    }
}
void scenario2_generation(int n, int gn, int un){
    Graph * g;
    std::vector<Edge> updates;
    int fi = 0;

    for(double p = 0.1; p <= 0.91; p+=0.05){
        for(int gi = 1; gi <= gn; gi++){
            g = new Graph;
            srand((unsigned)fi);
            fi++;
            g->generate("Gnp", n, p);
            g->saveToFile(numberedFilename("data/bfsfra/graph_", fi));
            cout << "Grafas " << p << " - " << gi << "\n";

            g->generateFullFillInsertions(&updates);
            g->saveUpdatesToFile(numberedAndVersionedFilename("data/bfsfra/updates_", 1, fi), updates);
            cout << "   Updates " << 1 << " (" << updates.size() << " inserts)\n";

            for(int ui = 2; ui <= un; ui++){
                std::random_shuffle(updates.begin(), updates.end());
                g->saveUpdatesToFile(numberedAndVersionedFilename("data/bfsfra/updates_", ui, fi), updates);
                cout << "   Updates " << ui << " (" << updates.size() << " inserts)\n";
            }
            updates.clear();
            delete g;
        }
    }
}
void scenario2(int gn, int un) {
    Graph * g;
    Graph * g2;
    std::vector<Edge> updates;
    std::vector<Edge> updates2;
    double sbfs_time;
    int updates_processed = 0;

    /* ------------------------------ Warm up ------------------------------ */
    Timer tt;
    tt.start();
    g = new Graph;
    (*g).readFromFile(numberedFilename("data/bfsMarked/experiment_graph_", 11));
    (*g).setSourceVertexByIndex(0);
    (*g).readUpdatesFromFile(numberedAndVersionedFilename("data/bfsMarked/experiment_updates_", 1, 11), &updates);
    bfs(*g);
    bfs(*g);
    delete g;
    updates.clear();
    tt.stop();
    /* ---------------------------- Warm up end ---------------------------- */

    Timer t1;
    Timer t2;
    for(int k = 1; k <= gn; k++){
        g = new Graph;
        g2 = new Graph;
        (*g).readFromFile(numberedFilename("data/bfsfra/graph_", k));
        g2->readFromFile(numberedFilename("data/bfsfra/graph_", k));
        for(int ui = 1; ui <= un; ui++){
            
            g->readUpdatesFromFile(numberedAndVersionedFilename("data/bfsfra/updates_", ui, k), &updates);
            g2->readUpdatesFromFile(numberedAndVersionedFilename("data/bfsfra/updates_", ui, k), &updates2);
            for(int svi = 0; svi < (*g).n; svi++){
                printf("G:%d UI:%d SVI:%d\n", k, ui, svi);
                g->setSourceVertexByIndex(svi);
                g2->setSourceVertexByIndex(svi);
                if(!g->reachableFromSource()){
                    cout << "G:" << k << " UI:" << ui << " SVI:" << svi << " Not reachable\n";
                    break;
                }

                t1.reset();
                t1.start();
                bfs2(*g);
                t1.stop();
                bfs2(*g2);
                sbfs_time = t1.getTime();
                cout << "G:" << k << " UI:" << ui << " SVI:" << svi << " " << sbfs_time << "  ";

                updates_processed = 0;
                for(int updateId = 0; updateId < updates.size(); updateId++){
                    //printf("Insert: %d\n", updateId);
                    t1.start();
                    franciosa_insert(updates[updateId]);
                    t1.stop();
                    t2.reset();
                    t2.start();
                    (*updates2[updateId].from).fs.push_back(updates2[updateId].to);
                    bfs2(*g2);
                    t2.stop();
                    cout << " " << t1.getTime() << " " << (t2.getTime()+sbfs_time);
                    updates_processed++;
                    if(t2.getTime()+sbfs_time < t1.getTime()) {
                        cout << " |";
                        break;
                    }
                }
                cout << "\n";
                //GrafÃƒÆ’Ã†â€™ÃƒÂ¢Ã¢â€šÂ¬Ã‚Â¦ÃƒÆ’Ã¢â‚¬Å¡Ãƒâ€šÃ‚Â³ atitaisymas
                for(int updateId = 0; updateId < updates_processed; updateId++){
                    (*updates[updateId].from).fs.pop_back();
                }
                bfs2(*g);
                for(int updateId = 0; updateId < updates_processed; updateId++){
                    (*updates2[updateId].from).fs.pop_back();
                }
            }
            updates.clear();
            updates2.clear();
        }
        delete g;
        delete g2;
    }

    cout << "\n";
}

void scenario3(){
    //Tests optimised Franciosa algorithm on 50 graphs and check if the results are the same as bfs
    Graph * g;
    Graph * g2;
    std::vector<Edge> updates;
    std::vector<Edge> updates2;
    std::list<Edge> ru;
    
    for(int k = 1; k <= 50; k++){
        g = new Graph;
        g2 = new Graph;
        (*g).readFromFile(numberedFilename("data/bfsMarked/experiment_graph_", k));
        (*g2).readFromFile(numberedFilename("data/bfsMarked/experiment_graph_", k));
        (*g).readUpdatesFromFile(numberedAndVersionedFilename("data/bfsMarked/experiment_updates_", 1, k), &updates);
        (*g2).readUpdatesFromFile(numberedAndVersionedFilename("data/bfsMarked/experiment_updates_", 1, k), &updates2);
        (*g).setSourceVertexByIndex(0);
        (*g2).setSourceVertexByIndex(0);

        bfs2(*g);
        bfs2(*g2);

        for(int updateId = 0; updateId < updates.size(); updateId++){
            franciosa_optimised_insert(updates[updateId], ru);
        }
        franciosa_optimised_flush(ru);

        for(int updateId = 1; updateId < updates2.size(); updateId++){
            (*updates2[updateId].from).fs.push_back(updates2[updateId].to);
        }

        if(g->spTreeToString() != g2->spTreeToString()){
            cout << "Optimised algorith works incorrectly!!!\n";
        }
        else cout << "Ok\n";
        updates.clear();
        updates2.clear();
        delete g;
        delete g2;
    }
}

int main() {
    //Test tester;
    //tester.run_tests();
    
    //std::ofstream out("out.txt");
    //std::cout.rdbuf(out.rdbuf()); //redirect std::cout to out.txt!

    //scenario2_generation(100, 5, 5);
    //scenario2(85, 5);

    //scenario3();
    scenario1(10, 1);
    
//    for(int v = 2; v <= 5; v++){
//        printf("V:%d\n", v);
//        std::ofstream out(numberedFilename("out.txt", v).c_str());
//        std::cout.rdbuf(out.rdbuf());
//        scenario1(50, v);
//    }

//    while(true){
//        cout << " --------- NEW ITERATION --------- " << time(0) << "\n";
//        testBfsVariants(50, 5);
//    }
    
    return 0;
}
