# Minimum Troop Placement in a Tree

## Language
The project is implemented in **C++17** using the C++ Standard Library.

## Description
The program finds the minimum number of troops required to satisfy the placement conditions on a tree. It uses Depth-First Search (DFS) to traverse the tree and Dynamic Programming (DP) to compute the minimum cost. It also reconstructs and displays an optimal troop allocation for the vertices.

## Input Format
- The first input is an integer `n`, the number of vertices in the tree.
- The next `n - 1` inputs are pairs of integers `u v`, representing undirected edges between vertices `u` and `v`.

Vertices are numbered from `1` to `n`. The input graph must be a tree.

## Output Format
The program displays:
- The minimum number of troops required, or a message indicating that no valid assignment exists.
- The troop allocation for each vertex.

## Compilation and Execution

Save the source code as `main.cpp`.

Compile with `g++`:

```bash
g++ -std=c++17 -O2 -o troop_placement main.cpp
```

Run on **Linux/macOS**:

```bash
./troop_placement
```

Run on **Windows**:

```powershell
.\troop_placement.exe
```

Enter the number of vertices and then the `n - 1` edges when prompted.

## Example

### Input
```text
5
1 2
1 3
2 4
2 5
```

### Output format
```text
Minimum number of troops: <minimum>

Troop allocation:
Vertex 1: <troop count>
Vertex 2: <troop count>
...
```

The program prints prompts while reading input, so the actual console output also includes those prompts.
