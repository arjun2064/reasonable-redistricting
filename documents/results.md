# Reasonable Redistricting Report

## Algorithms

### BFS Traversal
BFS traversal was used to find the degree of separation between separate presincts to ensure that the distribution of precincts in each state district was natural. The traversal took in 2 parameters, being the node that we start at and the node that we end at, usually differing in political leaning. It returned the number of steps to get from the first precinct to the second one. Due to the nature of state distributions, it would be impossible to have a disconnected graph, but measures are taken to prevent the program from breaking in the case of a state with disconnected precincts. This image (https://github-dev.cs.illinois.edu/cs225-sp22/haoqi3-harryyz2-bl29/blob/main/documents/wisconsin.png) shows a the distribution of precincts in Wisconsin which this project attempts to parse into districts. BFS successfully reported the distances between precincts shown here (https://github-dev.cs.illinois.edu/cs225-sp22/haoqi3-harryyz2-bl29/blob/main/documents/bfs_testing.png). These tests include directly linked nodes, random nodes, and unconnected nodes.

### Minimum Spanning Tree (Covered)

### Uniform Spanning Tree (Uncovered)
The uniform spanning tree algorithm took in a graph, and from that generated a random spanning tree to ensure the fairness of the distribution

MST and UST will then be used in the ReCom algorithm as follows:
1. start with a seed partitioning of precincts into n districts
2. randomly choose 2 districts
3. merge 2 districts
4. assign random weights to subgraph
5. draw minimum spanning tree on subgraph
6. cut an edge of spanning tree so that resulting partitions have balanced populations
7. calculate metric for current plan eg. democratic/republican
8. record this sample plan and metric
9. repeat step 2 for k samples
10. plot all samples on graph of number of plans against metric eg. how democratic/republican leaning a plan is.
11. obtain an ideal redistricting plan from the mean of this distribution. we can also compare proposed plans to this distribution to indicate if there's gerrymandering
