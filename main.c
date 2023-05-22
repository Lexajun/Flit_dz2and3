#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#define MAX_NODES 100

typedef struct Graph_t {
    int nodes[MAX_NODES][MAX_NODES];
    int num_nodes;
} Graph_t;


void dfs(int node, int* visited,  Graph_t* graph) {
    visited[node] = 1;
    for (int i = 0; i < graph->num_nodes; i++) {
        if (graph->nodes[node][i] && !visited[i]) {
            dfs(i, visited, graph);
        }
    }
}

bool is_connected( Graph_t* graph) {
    int* visited = calloc(graph->num_nodes, sizeof(int));
    dfs(0, visited, graph);
    for (int i = 0; i < graph->num_nodes; i++) {
        if (!visited[i]) {
            free(visited);
            return false;
        }
    }
    free(visited);
    return true;
}

void read_graph(FILE* file,  Graph_t* graph) {
    char line[256];
    int row = 0;

    while (fgets(line, sizeof(line), file)) {
        int col = 0;
        char* token = strtok(line, " ");
        while (token) {
            int value = atoi(token);
            graph->nodes[row][col++] = value;
            if (col-1 <= row) {
                graph->nodes[col-1][row] = value;
            }
            token = strtok(NULL, " ");
        }
        row++;
    }

    graph->num_nodes = row;
}

void write_dot(FILE* file,  Graph_t* graph) {
    fprintf(file, "digraph G {\n");
    for (int i = 0; i < graph->num_nodes; i++) {
        fprintf(file, "  %d;\n", i);
    }
    for (int i = 0; i < graph->num_nodes; i++) {
        for (int j = i; j < graph->num_nodes; j++) {
            if (graph->nodes[i][j]) {
                fprintf(file, "  %d -> %d [dir=none, label=%d];\n", i, j, graph->nodes[i][j]);
            }
        }
    }
    fprintf(file, "}\n");
}


int main() {
     Graph_t graph;

    FILE* file = fopen("./test.txt", "r");
    if (file == NULL) {
        fprintf(stderr, "Error. Unable to open file.\n");
        return 1;
    }
    read_graph(file, &graph);
    fclose(file);

    if (is_connected(&graph))
        puts("Graph is connected");
    else
        puts("Graph is NOT connected");

    file = fopen("Graph.dot", "w");
    if (file == NULL) {
        fprintf(stderr, "Error. Unable to open file.\n");
        return 1;
    }
    write_dot(file, &graph);
    fclose(file);
    system("dot -Tpng Graph.dot -o Graph.png");
    return 0;
}