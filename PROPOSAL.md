# Reasonable Redistricting

## Leading Question

>How do we divide a state’s voting precincts into groups (known as districts) such that districts are fairly distributed among Republicans and Democrats?

Subquestions related to our leading question include:
- Given all possible redistricting plans, what is the natural distribution of those plans?
- Given a proposed redistricting plan, how can we determine if the plan is biased so as to detect gerrymandering?

To answer these questions, we aim to model the voting precincts as a graph problem and implement the redistricting algorithm in Recombination: A Family of Markov Chains for Redistricting (
https://hdsr.mitpress.mit.edu/pub/1ds8ptxu/release/4). The metric we calculate from each redistricting plan is the percentage of Republican and Democrat votes in the 2018 US House Election. We then plot the natural distribution of the metric from the sampled redistricting plans. Plans closer to the mean are considered "fairer". We then make a variation of the algorithm by using a uniform spanning tree instead of a minimum spanning tree and compare the results of the two approaches.

## Dataset

We use the Wisconsin Voting District Dataset available at from the US Census Bureu: (https://www.census.gov/geo/partnerships/pvs/partnership20v1/st55_wi.html). This dataset contains Shapefiles which describe the geometric edges of each precinct.

We will plot these Shapefiles in a viewer over the map of Wisconsin to ensure that the boundaries of precincts make sense and drop any incorrect boundaries. eg. there shouldn't any precincts only over a body of water.

To obtain our graph of precincts, we will use `maup` (https://github.com/mggg/maup) to determine which precincts share a border. `maup` also will also identify borders that have gaps which would fail to map to an adjacency in our graph. Our graph will contain:
- nodes: precincts
- edges: when a precinct shares a border with another precinct/two precincts are adjacent
- 7078 nodes and 39056 edges
- stored as an adjacency list in json

```json
{
    "nodes": [
        {
            "Precinct": "Adams - C 0001",
            ...more precinct data...
        },
        {
            "Precinct": "Adams - C 0002",
            ...
        },
        {
            "Precinct": "Adams - C 0003",
            ...
        },
        ...
    ],
    "adjacency": [
        // adjacency for precinct 0
        [
            {
                "shared_perim": 6370.760172634003,
                "id": 4
            },
            {
                "shared_perim": 1459.9667971177569,
                "id": 1
            },
        ], 
        // adjacency for precinct 1
        [
            {
                "shared_perim": 1459.9667971177569,
                "id": 0
            },
        ]
        ...
    ]
}
```

## Algorithms

### BFS Traversal
Traverse through all precincts

### Minimum Spanning Tree (Covered)
- Prim's Algorithm
- Input: Subgraph of precincts where two districts have been merged
- Output: MST
- Big O: O(|E| log |V|)
- Note: weights on the minimum spanning tree are randomly assigned as stated in the paper so as to sample the space of all the ways to partition a subgraph of precincts

### Uniform Spanning Tree (Uncovered)
- Wilson’s algorithm (https://en.wikipedia.org/wiki/Loop-erased_random_walk)
- Input: Subgraph of precincts where two districts have been merged
- Output: UST
- Big O: O(E^3)

MST and UST will then be used in the ReCom algorithm as follows:
1. start with a seed partitioning of precincts into n districts
2. randomly choose 2 districts
3. merge 2 districts
4. assign random weights to subgraph
5. draw spanning tree on subgraph
6. cut an edge of spanning tree so that resulting partitions have balanced populations
7. calculate metric for current plan eg. democratic/republican
8. record this sample plan and metric
9. repeat step 2 for k samples
10. plot all samples on graph of number of plans against metric eg. how democratic/republican leaning a plan is.
11. obtain an ideal redistricting plan from the mean of this distribution. we can also compare proposed plans to this distribution to indicate if there's gerrymandering

## Timeline
- Week 1 - Mar 28
    - Data cleaning
    - Implement BST
- Week 2 - Apr 4
    - Implement MST
    - Implement tests for algorithms
- Week 3 - Apr 11 (mid project check in) One of the algos written
    - Use MST in ReCom Algorithm
- Week 4 - Apr 18
    - Implement UST and 
- Week 5 - Apr 25
    - Writeup and video presentation
- Week 6 - May 2 (final submission)
    - Buffer for any overflows in time estimates
