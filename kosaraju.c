#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_VERTICES 100

// Structure to represent a graph using adjacency list
typedef struct Graph {
    int vertices;
    int** adjMatrix;    // Adjacency matrix representation
    int* adjList[MAX_VERTICES];  // Adjacency list representation
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
void printSCCs(Graph* graph);

// Stack operations
void push(Stack* stack, int item);
int pop(Stack* stack);
bool isEmpty(Stack* stack);

/**
 * KOSARAJU-SHARIR ALGORITHM IMPLEMENTATION
 * 
 * This implementation demonstrates the complete Kosaraju-Sharir algorithm
 * for finding strongly connected components in a directed graph.
 * 
 * The algorithm consists of three phases:
 * 1. Perform DFS on original graph and record finish times
 * 2. Create transpose graph (reverse all edges)
 * 3. Perform DFS on transpose graph in reverse order of finish times
 */

int main() {
    printf("=== KOSARAJU-SHARIR ALGORITHM DEMONSTRATION ===\n\n");
    
    // Create a sample graph for demonstration
    // This graph has the following structure:
    //   0 → 1 → 2
    //   ↑       ↓
    //   3 ← 4 ← 2
    //   ↓
    //   5 → 6
    
    Graph* graph = createGraph(7);
    
    // Adding edges to create strongly connected components
    addEdge(graph, 0, 1);
    addEdge(graph, 1, 2);
    addEdge(graph, 2, 4);
    addEdge(graph, 4, 3);
    addEdge(graph, 3, 0);    // SCC 1: {0, 1, 2, 3, 4}
    
    addEdge(graph, 3, 5);
    addEdge(graph, 5, 6);    // SCC 2: {5}, SCC 3: {6}
    
    printf("Original Graph:\n");
    printGraph(graph);
    
    printf("\nFinding Strongly Connected Components using Kosaraju-Sharir Algorithm:\n");
    printSCCs(graph);
    
    // Free memory
    free(graph);
    
    return 0;
}

/**
 * Create a new graph with given number of vertices
 */
Graph* createGraph(int vertices) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->vertices = vertices;
    
    // Initialize adjacency matrix
    graph->adjMatrix = (int**)malloc(vertices * sizeof(int*));
    for (int i = 0; i < vertices; i++) {
        graph->adjMatrix[i] = (int*)calloc(vertices, sizeof(int));
    }
    
    // Initialize adjacency list
    for (int i = 0; i < vertices; i++) {
        graph->adjList[i] = (int*)malloc(vertices * sizeof(int));
        graph->adjListSize[i] = 0;
    }
    
    return graph;
}

/**
 * Add an edge from src to dest
 */
void addEdge(Graph* graph, int src, int dest) {
    // Add to adjacency matrix
    graph->adjMatrix[src][dest] = 1;
    
    // Add to adjacency list
    graph->adjList[src][graph->adjListSize[src]++] = dest;
}

/**
 * Phase 1 DFS: Traverse the graph and fill the stack with vertices
 * in order of their finish times (vertex with latest finish time on top)
 */
void DFS_phase1(Graph* graph, int vertex, bool visited[], Stack* stack) {
    visited[vertex] = true;
    printf("  Visiting vertex %d (Phase 1)\n", vertex);
    
    // Recursively visit all adjacent vertices
    for (int i = 0; i < graph->adjListSize[vertex]; i++) {
        int adjVertex = graph->adjList[vertex][i];
        if (!visited[adjVertex]) {
            DFS_phase1(graph, adjVertex, visited, stack);
        }
    }
    
    // Push current vertex to stack after visiting all its descendants
    push(stack, vertex);
    printf("  Finished processing vertex %d (Phase 1) - pushed to stack\n", vertex);
}

/**
 * Phase 2 DFS: Traverse the transpose graph and mark vertices
 * belonging to the same strongly connected component
 */
void DFS_phase2(Graph* transposeGraph, int vertex, bool visited[], int component[], int compNum) {
    visited[vertex] = true;
    component[vertex] = compNum;
    printf("  Vertex %d belongs to SCC %d\n", vertex, compNum);
    
    // Recursively visit all adjacent vertices in transpose graph
    for (int i = 0; i < transposeGraph->adjListSize[vertex]; i++) {
        int adjVertex = transposeGraph->adjList[vertex][i];
        if (!visited[adjVertex]) {
            DFS_phase2(transposeGraph, adjVertex, visited, component, compNum);
        }
    }
}

/**
 * Create transpose graph (reverse all edges)
 */
Graph* getTranspose(Graph* graph) {
    Graph* transpose = createGraph(graph->vertices);
    
    for (int v = 0; v < graph->vertices; v++) {
        for (int i = 0; i < graph->adjListSize[v]; i++) {
            int adjVertex = graph->adjList[v][i];
            addEdge(transpose, adjVertex, v);  // Reverse the edge
        }
    }
    
    return transpose;
}

/**
 * Main Kosaraju-Sharir algorithm implementation
 */
void kosaraju(Graph* graph) {
    Stack stack = { .top = -1 };
    bool* visited = (bool*)calloc(graph->vertices, sizeof(bool));
    int* component = (int*)malloc(graph->vertices * sizeof(int));
    
    printf("\n--- PHASE 1: DFS on Original Graph ---\n");
    // Phase 1: Fill vertices in stack according to their finishing times
    for (int i = 0; i < graph->vertices; i++) {
        if (!visited[i]) {
            DFS_phase1(graph, i, visited, &stack);
        }
    }
    
    printf("\nStack contents (finish time order): ");
    for (int i = 0; i <= stack.top; i++) {
        printf("%d ", stack.items[i]);
    }
    printf("\n");
    
    // Create transpose graph
    printf("\n--- PHASE 2: Creating Transpose Graph ---\n");
    Graph* transpose = getTranspose(graph);
    printf("Transpose graph created successfully\n");
    
    printf("\n--- PHASE 3: DFS on Transpose Graph ---\n");
    // Reset visited array for second DFS
    for (int i = 0; i < graph->vertices; i++) {
        visited[i] = false;
    }
    
    int componentCount = 0;
    
    // Phase 2: Process vertices in order stored in stack
    while (!isEmpty(&stack)) {
        int vertex = pop(&stack);
        if (!visited[vertex]) {
            printf("Starting new SCC (Component %d) from vertex %d:\n", componentCount, vertex);
            DFS_phase2(transpose, vertex, visited, component, componentCount);
            componentCount++;
            printf("\n");
        }
    }
    
    // Print results
    printf("--- RESULTS ---\n");
    printf("Total Strongly Connected Components found: %d\n\n", componentCount);
    
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

/**
 * Print the graph structure
 */
void printGraph(Graph* graph) {
    printf("Graph with %d vertices:\n", graph->vertices);
    for (int v = 0; v < graph->vertices; v++) {
        printf("Vertex %d -> ", v);
        for (int i = 0; i < graph->adjListSize[v]; i++) {
            printf("%d ", graph->adjList[v][i]);
        }
        printf("\n");
    }
}

/**
 * Wrapper function to find and print strongly connected components
 */
void printSCCs(Graph* graph) {
    kosaraju(graph);
}

// Stack operations
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