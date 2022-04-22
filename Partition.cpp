#include "Partition.h"
#include "DisjointSets.h"
#include <unordered_map>
#include <stdexcept>
#include <random>
#include <algorithm>
#include <queue>
#include <limits.h>

typedef std::pair<int, int> ipair;
using std::make_pair;
using std::unordered_map;
using std::priority_queue;

Partition::Partition(Graph* graph, int numDistricts): graph(graph), numDistricts(numDistricts) {
    randomJoinInitialize();
    initializeDistrictAdjacencies();
    allocateCaches();
}

/**
 * Initializes partition by randomly joining adjacent precincts
 * until there is the right number of districts
 */
void Partition::randomJoinInitialize() {
    int numPrecincts = graph->numPrecincts();
    if (numPrecincts < numDistricts) {
        throw std::runtime_error("Too few precincts");
    }

    // create list of edges
    vector<ipair> edges;
    for (int i = 0; i < numPrecincts; i++) {
        for (auto j : graph->getEdges()[i]) {
            edges.push_back(make_pair(i, j));
        }
    }

    // shuffle edge list
    static auto rng = std::default_random_engine(time(NULL));
    std::shuffle(edges.begin(), edges.end(), rng);
    
    // merge randomly until you have the right number of districts
    DisjointSets dsets(numPrecincts);
    unsigned edgeIdx = 0;
    while (dsets.getNumSets() > numDistricts) {
        if (edgeIdx > edges.size()) {
            throw std::runtime_error("Graph consists of too many distinct islands");
        }
        ipair edge = edges[edgeIdx++];
        dsets.join(edge.first, edge.second);
    }

    // collect each the precincts of each district into vectors
    unordered_map<int, int> idxMap;
    int districtIdx = 0;
    districtToPrecincts.resize(numDistricts);
    for (int i=0; i<numPrecincts; i++) {
        int rootIdx = dsets.find(i);
        if (!idxMap.count(rootIdx)) {
            idxMap[rootIdx] = districtIdx++;
        }
        precinctToDistrict[i] = idxMap[rootIdx];
        districtToPrecincts[idxMap[rootIdx]].push_back(i);
    }
}

// Initialize the district adjacency sets
void Partition::initializeDistrictAdjacencies() {
    for (int district = 0; district < numDistricts; district++) {
        districtAdjacencies[district].clear();
        for (int precinct : districtToPrecincts[district]) {
            for (int adjacentPrecinct : graph->getEdges()[precinct]) {
                int adjacentDistrict = precinctToDistrict[adjacentPrecinct];
                if (adjacentDistrict != district) {
                    districtAdjacencies[district].insert(adjacentDistrict);
                }
            }
        }
    }
}

// Remove all adjacency relationships for a particular district
void Partition::removeDistrictAdjacencies(int district) {
    for (int adjacentDistrict : districtAdjacencies[district]) {
        districtAdjacencies[adjacentDistrict].erase(district);
    }
    districtAdjacencies[district].clear();
}

// Calculate and add all adjacency relationships for a particular district
void Partition::addDistrictAdjacencies(int district) {
    districtAdjacencies[district].clear();
    for (int precinct : districtToPrecincts[district]) {
        for (int adjacentPrecinct : graph->getEdges()[precinct]) {
            int adjacentDistrict = precinctToDistrict[adjacentPrecinct];
            if (adjacentDistrict != district) {
                districtAdjacencies[district].insert(adjacentDistrict);
                districtAdjacencies[adjacentDistrict].insert(district);
            }
        }
    }
}

void Partition::allocateCaches() {
    visitedCache.resize(graph->numPrecincts());
    keyCache.resize(graph->numPrecincts());
    parentCache.resize(graph->numPrecincts());
    treeCache.resize(graph->numPrecincts());
}

// Do a random recombination of two districts
void Partition::recombination() {
    // todo some other time
}

// Do a recombination of two particular districts
void Partition::recombination(int districtA, int districtB) {
    removeDistrictAdjacencies(districtA);
    removeDistrictAdjacencies(districtB);

    // Merging both districts into districtA
    for (int precinct : districtToPrecincts[districtB]) {
        precinctToDistrict[districtB] = districtA;
    }
    districtToPrecincts[districtA].insert(
        districtToPrecincts[districtA].end(),
        districtToPrecincts[districtA].begin(),
        districtToPrecincts[districtB].end()
    );

    addDistrictAdjacencies(districtA);
    addDistrictAdjacencies(districtB);
}


// Calculates the minimum spanning tree of a particular district
void Partition::minSpanningTree(int district) {
    for (int precinct : districtToPrecincts[district]) {
        keyCache[precinct] = INT_MAX;
        parentCache[precinct] = -1;
        treeCache[precinct].clear();
    }
    // pair structure should be (weight, index)
    priority_queue<ipair, vector<ipair>, std::greater<ipair>> pq;
    int startingPrecinct = districtToPrecincts[district][0];
    keyCache[startingPrecinct] = 0;
    pq.push(make_pair(keyCache[startingPrecinct], startingPrecinct));
    while (!pq.empty()) {
        // Do stuff
    }
}

// void Partition::randomlyAssignWeights(int district) {
//     for (int precinct : districtToPrecincts[district]) {
//         auto& edges = graph->getEdges();
//         for (unsigned i = 0; i < edges[precinct].size(); i++) {
//             int neighbor = edges[precinct][i];
//             // Having two different weights for the same edge depending on which
//             // side you go from shouldn't change the distribution because prim's
//             // algorithm for creating an mst will only ever check the weight
//             // in one direction
//             if (precinctToDistrict[neighbor] == district) {
//                 weights[precinct][i] = rand();
//             }
//         }
//     }
// }