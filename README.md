# tictactoe
School Project

This repository contains:

## 1. Tic-Tac-Toe Game
A classic tic-tac-toe game implementation in C.

### Build and Run
```bash
gcc -o tictactoe main.c
./tictactoe
```

## 2. Kosaraju-Sharir Algorithm
A comprehensive implementation and explanation of the Kosaraju-Sharir algorithm for finding strongly connected components in directed graphs.

### Files:
- `kosaraju_algorithm.md` - Detailed explanation of the algorithm
- `kosaraju.c` - Complete implementation with educational comments
- `kosaraju_examples.c` - Multiple examples demonstrating the algorithm

### Build and Run Algorithm Examples
```bash
# Basic implementation
gcc -o kosaraju kosaraju.c
./kosaraju

# Multiple examples
gcc -o kosaraju_examples kosaraju_examples.c
./kosaraju_examples
```

### Algorithm Overview
The Kosaraju-Sharir algorithm finds strongly connected components (SCCs) in a directed graph in O(V + E) time using two DFS passes:
1. DFS on original graph to get finish times
2. DFS on transpose graph in reverse finish time order

Each DFS tree in the second pass represents one strongly connected component.
