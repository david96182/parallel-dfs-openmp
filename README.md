<h1 style="font-size:300%;" align="center" >Parallel DFS OpenMp</h1>

  <p align="center">
     Sequential and parallel implementation of depth-first search algorithm using OpenMP.
    <br />
</p>
## Details

The implemented code finds the path in DFS given a source node and a destination node, for this it reads a graph from the graph folder, where there are 3 examples with different sizes.

## Execution Details

1. Compile dfs.cpp

   ```
   g++ -o dfs -fopenmp dfs.cpp
   ```

   

2. Run

   ```
   ./dfs [threads] [from] [to]
   ```

   threads: is the number of threads that the parallel algorithm will use

   from: is the root node or source node

   to: is the destination node, the one sought