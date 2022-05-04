#pragma once

#include <vector>
#include <unordered_set>
#include "Graph.h"
#include <functional>

using std::vector;
using std::unordered_set;
class Partition {
    public:
        enum SpanningTreeAlgorithm { MST, WILSON };
        
        Partition(Graph* graph, int numDistricts, SpanningTreeAlgorithm treeAlgorithm);
        void recombination();
        void recombination(int districtA, int districtB);
        // calculate mean median score for current partition state
        float getMeanMedian();
        void minSpanningTree(int district, std::function<int(int, int)> getEdgeWeight, int startingPrecinctIdx = 0);
        void wilsonTree(int district);

        vector<vector<int>>& getTreeCache();
    private:
        Graph* graph;
        int numDistricts;
        SpanningTreeAlgorithm treeAlgorithm;

        vector<vector<int>> districtToPrecincts;
        vector<int> precinctToDistrict;
        vector<unordered_set<int>> districtAdjacencies;

        // Some preallocated arrays used as a cache during recombination
        // vector<vector<int>> weights;
        vector<int> visitedCache;
        vector<int> keyCache;
        vector<int> parentCache;
        vector<vector<int>> treeCache;
        vector<int> populationCache;

        void randomJoinInitialize();
        void initializeDistrictAdjacencies();
        void removeDistrictAdjacencies(int district);
        void addDistrictAdjacencies(int district);
        void allocateCaches();

        int calculatePopulations(int precinct);
        void dfsRebuild(int district, int precinct);
        void dfsRebuild(int district, int precinct, int exclude);
};