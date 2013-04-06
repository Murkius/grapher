#include "Graph.h"

using namespace std;

Graph::Graph() {
    srand((unsigned)time(0));
}

void Graph::generateByGnp(int n, float p) {
    for(int i = 0; i < n; i++){
        graph[i].identifier = i;
        graph[i].fs_size = 0;
        //TODO: graph[i].bs_size = 0;
    }
    
    for(int i = 0; i < n; i++) { //from
        for(int j = 0; j < n; j++) { //to
            if((float)rand()/(float)RAND_MAX < p) {
                graph[i].fs[graph[i].fs_size++] = &graph[j];
            }            
        }
    }
}
void Graph::readFromFile(const char filename[]) {
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
        graph[c].distance_to_source = 0;
        fscanf(fp, "%d %d", &(graph[c].identifier), &(graph[c].fs_size));
        for(int i = 0; i < graph[c].fs_size; i++) {
            fscanf(fp, "%d", &t);
            graph[c].fs[i] = &(graph[t-1]); //TODO: pataisyti
        }
    }

    fclose(fp);
}

void Graph::setSourceVertexByIndex(int i) {
    sourceVertex = &graph[i];
}
struct Vertex* Graph::getSourceVertex() {
    return sourceVertex;
}

string Graph::spTreeToString() { 
    /*
        Eilutė gaminama tik pagal Vertex.parent; remiasi prielaida, kad graph[0] yra source viršūnė
    */
    struct Vertex* pgraph[n];
    for(int i = 0; i < n; i++) {
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

