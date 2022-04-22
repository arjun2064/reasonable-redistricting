#pragma once

#include <vector>
#include "Graph.h"

using std::vector;
class Partition {
    public:
        Partition(Graph graph, int numDistricts);
        void randomJoinInitialize();
    private:
        Graph* graph;
        int numDistricts;
        vector<vector<int>> districts;
        vector<vector<int>> weights;
        void recom();
        void minSpanningTree(int district);
        void uniformSpanningTree(int district);
};