#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_VERTICES 100

// Structure to represent a graph using adjacency list
typedef struct Graph {
    int vertices;
    int* adjList[MAX_VERTICES];
    int adjListSize[MAX_VERTICES];
} Graph;

// Stack structure for storing vertices by finish time
typedef struct Stack {
    int items[MAX_VERTICES];
    int top;
} Stack;

// Function prototypes
Graph* createGraph(int vertices);
void addEdge(Graph* graph, int src, int dest);
void DFS_phase1(Graph* graph, int vertex, bool visited[], Stack* stack);
void DFS_phase2(Graph* transposeGraph, int vertex, bool visited[], int component[], int compNum);
Graph* getTranspose(Graph* graph);
void kosaraju(Graph* graph);
void printGraph(Graph* graph);
void runExample1();
void runExample2();

// Stack operations
void push(Stack* stack, int item);
int pop(Stack* stack);
bool isEmpty(Stack* stack);

int main() {
    printf("=== KOSARAJU-SHARIR ALGORITHM EXAMPLES ===\n\n");
    
    runExample1();
    printf("\n============================================================\n\n");
    runExample2();
    
    return 0;
}

/**
 * Example 1: Simple graph with clear SCCs
 */
void runExample1() {
    printf("EXAMPLE 1: Simple Graph with Multiple SCCs\n");
    printf("Graph structure:\n");
    printf("  0 → 1 → 2\n");
    printf("  ↑       ↓\n");
    printf("  3 ← 4 ← 2\n");
    printf("  ↓\n");
    printf("  5 → 6\n\n");
    
    Graph* graph = createGraph(7);
    addEdge(graph, 0, 1);
    addEdge(graph, 1, 2);
    addEdge(graph, 2, 4);
    addEdge(graph, 4, 3);
    addEdge(graph, 3, 0);    // Forms SCC: {0, 1, 2, 3, 4}
    addEdge(graph, 3, 5);
    addEdge(graph, 5, 6);    // Forms SCCs: {5}, {6}
    
    kosaraju(graph);
    free(graph);
}

/**
 * Example 2: More complex graph
 */
void runExample2() {
    printf("EXAMPLE 2: Complex Graph with Multiple SCCs\n");
    printf("This example demonstrates a more complex graph structure.\n\n");
    
    Graph* graph = createGraph(8);
    
    // First SCC: {0, 1, 2}
    addEdge(graph, 0, 1);
    addEdge(graph, 1, 2);
    addEdge(graph, 2, 0);
    
    // Second SCC: {3, 4}
    addEdge(graph, 3, 4);
    addEdge(graph, 4, 3);
    
    // Connections between SCCs
    addEdge(graph, 2, 3);  // From first SCC to second SCC
    addEdge(graph, 4, 5);  // From second SCC to vertex 5
    
    // Third SCC: {5, 6, 7}
    addEdge(graph, 5, 6);
    addEdge(graph, 6, 7);
    addEdge(graph, 7, 5);
    
    printGraph(graph);
    kosaraju(graph);
    free(graph);
}

Graph* createGraph(int vertices) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->vertices = vertices;
    
    for (int i = 0; i < vertices; i++) {
        graph->adjList[i] = (int*)malloc(vertices * sizeof(int));
        graph->adjListSize[i] = 0;
    }
    
    return graph;
}

void addEdge(Graph* graph, int src, int dest) {
    graph->adjList[src][graph->adjListSize[src]++] = dest;
}

void DFS_phase1(Graph* graph, int vertex, bool visited[], Stack* stack) {
    visited[vertex] = true;
    printf("  Phase 1 - Visiting: %d\n", vertex);
    
    for (int i = 0; i < graph->adjListSize[vertex]; i++) {
        int adjVertex = graph->adjList[vertex][i];
        if (!visited[adjVertex]) {
            DFS_phase1(graph, adjVertex, visited, stack);
        }
    }
    
    push(stack, vertex);
    printf("  Phase 1 - Finished: %d (pushed to stack)\n", vertex);
}

void DFS_phase2(Graph* transposeGraph, int vertex, bool visited[], int component[], int compNum) {
    visited[vertex] = true;
    component[vertex] = compNum;
    printf("  Phase 2 - Vertex %d → SCC %d\n", vertex, compNum);
    
    for (int i = 0; i < transposeGraph->adjListSize[vertex]; i++) {
        int adjVertex = transposeGraph->adjList[vertex][i];
        if (!visited[adjVertex]) {
            DFS_phase2(transposeGraph, adjVertex, visited, component, compNum);
        }
    }
}

Graph* getTranspose(Graph* graph) {
    Graph* transpose = createGraph(graph->vertices);
    
    for (int v = 0; v < graph->vertices; v++) {
        for (int i = 0; i < graph->adjListSize[v]; i++) {
            int adjVertex = graph->adjList[v][i];
            addEdge(transpose, adjVertex, v);
        }
    }
    
    return transpose;
}

void kosaraju(Graph* graph) {
    Stack stack = { .top = -1 };
    bool* visited = (bool*)calloc(graph->vertices, sizeof(bool));
    int* component = (int*)malloc(graph->vertices * sizeof(int));
    
    printf("PHASE 1: DFS on Original Graph\n");
    for (int i = 0; i < graph->vertices; i++) {
        if (!visited[i]) {
            DFS_phase1(graph, i, visited, &stack);
        }
    }
    
    printf("\nFinish Time Stack: ");
    for (int i = 0; i <= stack.top; i++) {
        printf("%d ", stack.items[i]);
    }
    printf("\n\n");
    
    Graph* transpose = getTranspose(graph);
    
    for (int i = 0; i < graph->vertices; i++) {
        visited[i] = false;
    }
    
    printf("PHASE 2: DFS on Transpose Graph\n");
    int componentCount = 0;
    
    while (!isEmpty(&stack)) {
        int vertex = pop(&stack);
        if (!visited[vertex]) {
            printf("New SCC %d starting from vertex %d:\n", componentCount, vertex);
            DFS_phase2(transpose, vertex, visited, component, componentCount);
            componentCount++;
            printf("\n");
        }
    }
    
    printf("RESULTS:\n");
    printf("Found %d Strongly Connected Components:\n", componentCount);
    
    for (int comp = 0; comp < componentCount; comp++) {
        printf("SCC %d: { ", comp);
        for (int v = 0; v < graph->vertices; v++) {
            if (component[v] == comp) {
                printf("%d ", v);
            }
        }
        printf("}\n");
    }
    
    free(visited);
    free(component);
    free(transpose);
}

void printGraph(Graph* graph) {
    printf("Graph adjacency list:\n");
    for (int v = 0; v < graph->vertices; v++) {
        printf("Vertex %d → ", v);
        for (int i = 0; i < graph->adjListSize[v]; i++) {
            printf("%d ", graph->adjList[v][i]);
        }
        if (graph->adjListSize[v] == 0) printf("(no outgoing edges)");
        printf("\n");
    }
    printf("\n");
}

void push(Stack* stack, int item) {
    if (stack->top < MAX_VERTICES - 1) {
        stack->items[++stack->top] = item;
    }
}

int pop(Stack* stack) {
    if (stack->top >= 0) {
        return stack->items[stack->top--];
    }
    return -1;
}

bool isEmpty(Stack* stack) {
    return stack->top == -1;
}