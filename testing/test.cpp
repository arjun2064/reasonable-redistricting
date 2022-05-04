#define CATCH_CONFIG_MAIN 
#include "catch.hpp"
#include "../Graph.h"
#include "../Partition.h"

#include <string>
#include <stack>
#include <set>

using std::cout;
using std::endl;
using std::stack;
using std::set;

// Checks tree connectivity
// Will infinite loop and crash if there is a cycle in the tree
bool treeIsValid(Partition& partition, int district) {
    set<int> treeContents;
    int root = partition.getDistrictPrecincts(district)[0];
    auto& treeCache = partition.getTreeCache();
    stack<int> stk;
    stk.push(root);
    while (!stk.empty()) {
        int precinct = stk.top();
        stk.pop();
        treeContents.insert(precinct);
        for (int child : treeCache[precinct]) {
            stk.push(child);
        }
    }
    set<int> districtContents;
    for (int precinct : partition.getDistrictPrecincts(district)) {
        districtContents.insert(precinct);
    }
    return treeContents == districtContents;
}

TEST_CASE("BFS close nodes") {
    Graph graph("cdata.txt");

    int distance1 = graph.BFS(0, 13);
    REQUIRE(2 == distance1);
}

TEST_CASE("BFS far nodes") {
    Graph graph("cdata.txt");

    int distance2 = graph.BFS(0, 2876);
    REQUIRE(56 == distance2);
}
    
TEST_CASE("BFS node not found in graph") {
    Graph graph("cdata.txt");
    int distance3 = graph.BFS(0, 8000);
    REQUIRE(-1 == distance3);
}

TEST_CASE("minSpanningTree() 1") {
    // test mst with graph from https://courses.engr.illinois.edu/cs225/sp2022/assets/notes/mst/prim_low_7.png
    // labelling of nodes from letters to num, eg. a = 0
    // starts at node 2 as per the example https://courses.engr.illinois.edu/cs225/sp2022/resources/mst/

    Graph graph;

    vector<Precinct> precincts = {
        {0, 0, 0, 0},
        {1, 0, 0, 0},
        {2, 0, 0, 0},
        {3, 0, 0, 0},
        {4, 0, 0, 0},
        {5, 0, 0, 0},
        {6, 0, 0, 0},
    };
    graph.setPrecincts(precincts);

    vector<vector<int>> edges = {
        {1, 4},
        {0, 3, 5, 6, 2},
        {1, 6},
        {1, 4},
        {0, 3, 5},
        {4, 1, 6},
        {5, 1, 2},
    };
    graph.setEdges(edges);

    Partition partition(&graph, 1, Partition::SpanningTreeAlgorithm::MST);

    partition.minSpanningTree(0, [&graph](int precinct1, int precinct2){
        static vector<vector<int>> weights = {
            {2, 3},
            {2, 5, 8, 6, 7},
            {7, 9},
            {5, 4},
            {3, 4, 10},
            {10, 8, 11},
            {11, 6, 9},
        };

        const vector<int>& edgeList = graph.getEdges()[precinct1];
        auto it = find(edgeList.begin(), edgeList.end(), precinct2);
        int indexOfWeightInPrecinct1WeightList = it - edgeList.begin();
        if (it == edgeList.end()) {
            throw std::runtime_error("Edge for "
                + std::to_string(precinct1) + ", " + std::to_string(precinct2)
                + " does not exist");
        }

        int weight = weights[precinct1].at(indexOfWeightInPrecinct1WeightList);

        return weight;
    }, 2);

    vector<vector<int>> expectedMST = {
        {4},
        {0, 5, 6},
        {1},
        {},
        {3},
        {},
        {},
    };

    REQUIRE(partition.getTreeCache() == expectedMST);
}

TEST_CASE("minSpanningTree() edge case 1 node") {
    Graph graph;

    vector<Precinct> precincts = {
        {0, 0, 0, 0},
    };
    graph.setPrecincts(precincts);

    vector<vector<int>> edges = {
        {},
    };
    graph.setEdges(edges);

    Partition partition(&graph, 1, Partition::SpanningTreeAlgorithm::MST);

    partition.minSpanningTree(0, [](int precinct1, int precinct2){
        return 0;
    });

    vector<vector<int>> expectedMST = {
        {},
    };

    REQUIRE(partition.getTreeCache() == expectedMST);
}

TEST_CASE("wilsonTree() 1") {
    // Test that wilsonTree() generates a valid tree

    Graph graph;

    vector<Precinct> precincts = {
        {0, 0, 0, 0},
        {1, 0, 0, 0},
        {2, 0, 0, 0},
        {3, 0, 0, 0},
        {4, 0, 0, 0},
        {5, 0, 0, 0},
        {6, 0, 0, 0},
    };
    graph.setPrecincts(precincts);

    vector<vector<int>> edges = {
        {1, 4},
        {0, 3, 5, 6, 2},
        {1, 6},
        {1, 4},
        {0, 3, 5},
        {4, 1, 6},
        {5, 1, 2},
    };
    graph.setEdges(edges);

    Partition partition(&graph, 1, Partition::SpanningTreeAlgorithm::WILSON);
    partition.wilsonTree(0);

    REQUIRE(treeIsValid(partition, 0));
}