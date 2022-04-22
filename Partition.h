#pragma once

#include <vector>
#include <unordered_set>
#include "Graph.h"

using std::vector;
using std::unordered_set;
class Partition {
    public:
        Partition(Graph* graph, int numDistricts);
        void recombination();
        void recombination(int districtA, int districtB);
    private:
        Graph* graph;
        int numDistricts;
        vector<vector<int>> districtToPrecincts;
        vector<int> precinctToDistrict;
        vector<unordered_set<int>> districtAdjacencies;
        vector<vector<int>> weights;

        void randomJoinInitialize();
        void initializeDistrictAdjacencies();
        void removeDistrictAdjacencies(int district);
        void addDistrictAdjacencies(int district);

        void minSpanningTree(int district);
        void uniformSpanningTree(int district);
};