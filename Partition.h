#pragma once

#include <vector>
#include "Graph.h"

using std::vector;
class Partition {
    public:
        Partition(Graph graph, int numDivisions);
    private:
        const Graph graph;
        const int numDivisions;
        vector<vector<int>> districts;
        vector<vector<int>> weights;
        void recom();
        void minSpanningTree(int district);
        void uniformSpanningTree(int district);
};