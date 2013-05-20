#include "Graph.h"

using namespace std;

Graph::Graph() {
    n = 0;
    srand((unsigned)time(0));
}
Graph::~Graph(){
    for(int i = 0; i < n; i++) {
        graph[i].fs.clear();
    }
    
    sourceVertex = 0;
    delete[] Graph::graph;
    n = 0;
}

void Graph::generateByGnp(int n, float p) {
    Graph::n = n;
    graph = new (nothrow) Vertex [n];
    if(graph == 0) {
        Graph::n = 0;
        cout << "Error while allocating graph\n";
        return;
    }
    for(int i = 0; i < n; i++){
        graph[i].identifier = i;
        graph[i].marked = false;
        graph[i].distance_to_source = -1;
        //TODO: graph[i].bs_size = 0;
    }
    
    int m = 0;
    for(int i = 0; i < n; i++) { //from
        for(int j = 0; j < n; j++) { //to
            if(i != j && (float)rand()/(float)RAND_MAX < p) {
                graph[i].fs.push_back(&graph[j]);
                m++;
            }            
        }
    }
}
bool Graph::reachableFromSource() {
    queue<struct Vertex*> Q; 
    struct Vertex* t, u;
    
    Q.push(getSourceVertex());
    (*getSourceVertex()).marked = true;
    while (not Q.empty()) {
        t = Q.front();
        Q.pop();
        for (int c = 0; c < (*t).fs.size(); c++) {
            if (not (*(*t).fs[c]).marked) {
                (*(*t).fs[c]).marked = true;
                Q.push((*t).fs[c]);
            }
        }
    }
    
    bool connective = true;
    for(int i = 0; i < n; i++) {
        if(graph[i].marked) {
            graph[i].marked = false;
        }
        else {
            connective = false;
        }
    }
    return connective;
}
void Graph::generate(string method, float param1, float param2) {
    //long c = 0;
    do {
        //c++;
        if(method == "Gnp") {
            generateByGnp((int)param1, param2);
        }
        setSourceVertexByIndex(0);
    } while(!reachableFromSource());
    //cout << "Sugeneruota is " << c << " karto\n";
}

void Graph::generateInsertions(int numberOfInsertions, std::vector<Edge> *insertions) {
    int from, to;
    bool suitableEdge;
    
    for(int i = 0; i < numberOfInsertions; i++) {
        do {
            suitableEdge = true;
            from = (int)round(((float)rand()/RAND_MAX)*(n-1));
            to = (int)round(((float)rand()/RAND_MAX)*(n-1));
            if(from == to) {    //Atmetame briaunas rodančias į tą pačią viršūnę iš kurios išėjo
                suitableEdge = false;
                continue;
            }
            for(int j = 0; j < graph[from].fs.size(); j++) { //Atmetame briaunas kurios jau egzistuoja grafe
                if(graph[from].fs[j] == &graph[to]) {
                    suitableEdge = false;
                    break;
                }
            }
            if(not suitableEdge) {
                continue;
            }
            for(int j = 0; j < (*insertions).size(); j++) { //Atmetame briaunas kurios jau egzistuoja pridedamų briaunų sąraše
                if((*insertions)[j].from == &graph[from] && (*insertions)[j].to == &graph[to]) {
                    suitableEdge = false;
                    break;
                }
            }
        } while(not suitableEdge);
        Edge e;
        e.from = &graph[from];
        e.to = &graph[to];
        e.operation = '+';
        (*insertions).push_back(e);
    }
}
void Graph::generateFullFillInsertions(std::vector<Edge> *insertions) {
    int from, to;
    bool edgeExist;

    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            if(j == i){
                continue;
            }
            edgeExist = false;
            for(int fsi = 0; fsi < graph[i].fs.size(); fsi++){
                if(graph[i].fs[fsi] == &graph[j]){
                    edgeExist = true;
                    continue;
                }
            }
            if(!edgeExist){
                Edge e;
                e.from = &graph[i];
                e.to = &graph[j];
                e.operation = '+';
                (*insertions).push_back(e);
            }
        }
    }
    std::random_shuffle((*insertions).begin(), (*insertions).end());
}

void Graph::readFromFile(string filename) {
    FILE *fp;
    int t;
    int * fs_size;

    filename = BASE_PATH+filename;
    fp = fopen(filename.c_str(), "r");
    if( fp == NULL ) {
        perror("Error while opening the file ");
        perror(filename.c_str());
        exit(EXIT_FAILURE);
    }

    fscanf(fp, "%d", &n);
    
    Graph::graph = new Vertex [n];
    fs_size = new int [n];
    
    std::vector<int> fs[n];
    
    for (int c = 0; c < n; c++) {
        graph[c].marked = false;
        graph[c].parent = 0;
        graph[c].distance_to_source = 0;
        fscanf(fp, "%d %d", &(graph[c].identifier), &fs_size[c]);

        for(int i = 0; i < fs_size[c]; i++) {
            fscanf(fp, "%d", &t);
            fs[c].push_back(t);
        }
    }
    
    for (int c = 0; c < n; c++) {
        for(int fsi = 0; fsi < fs_size[c]; fsi++) {
            for (int c2 = 0; c2 < n; c2++) {
                if(fs[c][fsi] == graph[c2].identifier) {
                    graph[c].fs.push_back(&graph[c2]);
                }
            }
        }
    }

    fclose(fp);
}
void Graph::saveToFile(string filename) {
    FILE *fp;

    filename = BASE_PATH+filename;
    fp = fopen(filename.c_str(), "w");
    if( fp == NULL ) {
        perror("Error while opening the file ");
        perror(filename.c_str());
        exit(EXIT_FAILURE);
    }
    
    fprintf(fp, "%d\n", n);
    
    for (int c = 0; c < n; c++) {
        fprintf(fp, "%d %d", graph[c].identifier, graph[c].fs.size());
        for(int i = 0; i < graph[c].fs.size(); i++) {
            fprintf(fp, " %d", (*graph[c].fs[i]).identifier);
        }
        fprintf(fp, " \n");
    }
    
    fclose(fp);
}
void Graph::readUpdatesFromFile(std::string filename, std::vector<Edge> *updates) {
    FILE *fp;
    int updatesNumber, fromId, toId;
    
    (*updates).clear();

    filename = BASE_PATH+filename;
    fp = fopen(filename.c_str(), "r");
    if( fp == NULL ) {
        perror("Error while opening the file ");
        perror(filename.c_str());
        exit(EXIT_FAILURE);
    }
    
    fscanf(fp, "%d\n", &updatesNumber);
    
    for (int c = 0; c < updatesNumber; c++) {
        Edge e;
        fscanf(fp, "%c %d %d\n", &e.operation, &fromId, &toId);
        for(int i = 0; i < n; i++) {
            if(graph[i].identifier == fromId) {
                e.from = &graph[i];
            }
            if(graph[i].identifier == toId) {
                e.to = &graph[i];
            }
        }
        (*updates).push_back(e);
    }
    
    fclose(fp);
}
void Graph::saveUpdatesToFile(string filename, std::vector<Edge> updates) {
    FILE *fp;

    filename = BASE_PATH+filename;
    fp = fopen(filename.c_str(), "w");
    if( fp == NULL ) {
        perror("Error while opening the file ");
        perror(filename.c_str());
        exit(EXIT_FAILURE);
    }
    
    fprintf(fp, "%d\n", updates.size());
    
    for (int c = 0; c < updates.size(); c++) {
        fprintf(fp, "%c %d %d\n", updates[c].operation, (*updates[c].from).identifier, (*updates[c].to).identifier);
    }
    
    fclose(fp);
}

void Graph::setSourceVertexByIndex(int i) {
    sourceVertex = &graph[i];
}
struct Vertex* Graph::getSourceVertex() {
    return sourceVertex;
}

int Graph::getEdgeNumber(){
    int en = 0;
    for(int i = 0; i < n; i++){
        en += graph[i].fs.size();
    }
    return en;
}

string Graph::spTreeToString() { 
    /*
        Eilutė gaminama tik pagal Vertex.parent; remiasi prielaida, kad graph[0] yra source viršūnė
    */
    struct Vertex* pgraph[n];
    for(int i = 0; i < n; i++) {
        if(graph[i].parent == 0 && &graph[i] != sourceVertex) {
            cout << "\nVirsunei " << graph[i].identifier << " nenustatyta tevine virsune!\n";
            return "";
        }
        pgraph[i] = &graph[i];
    }
    bool brotherFound;
    int baseId;
    for(int i = 1; i < n; i++){
        if((*(*pgraph[i]).parent).identifier == (*pgraph[i-1]).identifier) {
            continue;
        }
        for(int j = i+1; j < n; j++){
            if((*(*pgraph[j]).parent).identifier == (*pgraph[i-1]).identifier) {
                Vertex *temp = pgraph[i];
                pgraph[i] = pgraph[j];
                pgraph[j] = temp;
                break;
            } 
        }
        if((*(*pgraph[i]).parent).identifier == (*pgraph[i-1]).identifier) {
            continue;
        }
        
        baseId = i-1;
        brotherFound = false;
        while(baseId > 0 && !brotherFound){
            for(int j = i; j < n; j++){
                if((*pgraph[j]).parent == (*pgraph[baseId]).parent) {
                    brotherFound = true;
                    Vertex *temp = pgraph[i];
                    pgraph[i] = pgraph[j];
                    pgraph[j] = temp;
                    break;
                } 
            }
            baseId--;
        }
        if(!brotherFound) {
            cout << "\nNetinkama trumpiausio kelio medzio struktura!\n";
            return "";
        }
    }
    ostringstream content;
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < (*pgraph[i]).distance_to_source; j++) {
            content << "  ";
        }
        content << (*pgraph[i]).identifier << "\n";
    }
    return content.str();
}

