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
    minSpanningTree(districtA);
    calculatePopulations(districtToPrecincts[districtA][0]);

    addDistrictAdjacencies(districtA);
    addDistrictAdjacencies(districtB);
}


// Calculates the minimum spanning tree of a particular district
void Partition::minSpanningTree(int district) {
    for (int precinct : districtToPrecincts[district]) {
        visitedCache[precinct] = false;
        keyCache[precinct] = INT_MAX;
        parentCache[precinct] = -1;
        treeCache[precinct].clear();
    }
    const vector<vector<int>>& edges = graph->getEdges();

    // pair structure should be (weight, index)
    priority_queue<ipair, vector<ipair>, std::greater<ipair>> pq;
    // Starting precinct is the first precinct in the list
    int startingPrecinct = districtToPrecincts[district][0];
    keyCache[startingPrecinct] = 0;
    pq.push(make_pair(keyCache[startingPrecinct], startingPrecinct));
    while (!pq.empty()) {
        int precinct = pq.top().second;
        pq.pop();
        if (visitedCache[precinct])
            continue;
        visitedCache[precinct] = true;
        
        for (int i = 0; i < edges[precinct].size(); i++) {
            int neighbor = edges[precinct][i];
            // Check if neighbor is in correct district and unvisited
            if (precinctToDistrict[neighbor] == district && !visitedCache[neighbor]) {
                // Since the weight of every edge is only read once during the execution
                // of Prim's algorithm, choosing a random weight for the edge on the spot
                // has the exact same effect as assigning random weights to the graph
                // prior to running Prim's algorithm.
                int randomWeight = rand();
                if (randomWeight < keyCache[neighbor]) {
                    keyCache[neighbor = randomWeight];
                    pq.push(make_pair(randomWeight, neighbor));
                }
            }
        }
    }

    for (int precinct : districtToPrecincts[district]) {
        if (precinct == startingPrecinct) {
            if (parentCache[precinct] != -1) {
                throw std::runtime_error("Root node has parent");
            }
        } else {
            if (parentCache[precinct] == -1) {
                throw std::runtime_error("Tree is not connected");
            }
            treeCache[parentCache[precinct]].push_back(precinct);
        }
    }
}

// Cache the population size of each subtree
int Partition::calculatePopulations(int precinct) {
    populationCache[precinct] = graph->getPrecincts()[precinct].population;
    for (int child : treeCache[precinct]) {
        populationCache[precinct] += calculatePopulations(child);
    }
}