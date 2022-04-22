#include "Partition.h"
#include "DisjointSets.h"
#include <unordered_map>
#include <stdexcept>
#include <random>
#include <algorithm>

using std::make_pair;
using std::unordered_map;
typedef std::pair<int, int> ipair;

Partition::Partition(Graph* graph, int numDistricts): graph(graph), numDistricts(numDistricts) {
    // Initial contents of the weights don't matter, only the dimensions of the vectors.
    weights = graph->getEdges();

    randomJoinInitialize();
    initializeDistrictAdjacencies();
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
    auto rng = std::default_random_engine {};
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

// Do a random recombination of two districts
void Partition::recombination() {
    // todo
}

// Do a recombination of two particular districts
void Partition::recombination(int districtA, int districtB) {
    removeDistrictAdjacencies(districtA);
    removeDistrictAdjacencies(districtB);
}