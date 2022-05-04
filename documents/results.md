# Reasonable Redistricting Report

## Algorithms

### BFS Traversal


### Minimum Spanning Tree (Covered)

### Uniform Spanning Tree (Uncovered)

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
