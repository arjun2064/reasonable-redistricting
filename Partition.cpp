#include "Partition.h"
#include "DisjointSets.h"
#include <iostream>
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
using std::pair;
using std::sort;

Partition::Partition(Graph* graph, int numDistricts, SpanningTreeAlgorithm treeAlgorithm): graph(graph), numDistricts(numDistricts), treeAlgorithm(treeAlgorithm) {
    allocateCaches();
    partitionInitialize();
}

/**
 * Initializes partition by randomly joining adjacent precincts
 * until there is the right number of districts
 */
void Partition::partitionInitialize() {
    int numPrecincts = graph->numPrecincts();
    if (numPrecincts < numDistricts) {
        throw std::runtime_error("Too few precincts");
    }
    districtToPrecincts.resize(numDistricts);
    precinctToDistrict.resize(numPrecincts);

    for (int i=0; i<numPrecincts; i++) {
        districtToPrecincts[0].push_back(i);
        precinctToDistrict[i] = 0;
    }
    for (int i=1; i<numDistricts; i++) {
        int maxDistrict = 0;
        int maxPopulation = getPopulation(0);
        for (int j=1; j<i; j++) {
            int jPopulation = getPopulation(j);
            if (jPopulation > maxPopulation) {
                maxDistrict = j;
                maxPopulation = jPopulation;
            }
        }
        recombination(maxDistrict, i, .5);
    }
}

void Partition::allocateCaches() {
    visitedCache.resize(graph->numPrecincts());
    keyCache.resize(graph->numPrecincts());
    parentCache.resize(graph->numPrecincts());
    treeCache.resize(graph->numPrecincts());
    populationCache.resize(graph->numPrecincts());
}

// Do a random recombination of two districts
void Partition::recombination() {
    vector<ipair> edges;
    for(int i = 0; i < (int) graph->getEdges().size(); ++i) {
        for(int j: graph->getEdges()[i]){
            // ensure precincts are from different districts
            if (precinctToDistrict[i] != precinctToDistrict[j]) {
                edges.push_back({i, j});
            }
        }
    }

    ipair randomEdge = edges[rand() % (edges.size())];
    int district1 = precinctToDistrict[randomEdge.first];
    int district2 = precinctToDistrict[randomEdge.second];

    while (!recombination(district1, district2, 0.05));
}

// Do a recombination of two particular districts
bool Partition::recombination(int districtA, int districtB, double tolerance) {
    // Merging both districts into districtA
    for (int precinct : districtToPrecincts[districtB]) {
        precinctToDistrict[precinct] = districtA;
    }
    auto tempA = districtToPrecincts[districtA];
    auto tempB = districtToPrecincts[districtB];
    districtToPrecincts[districtA].insert(
        districtToPrecincts[districtA].end(),
        districtToPrecincts[districtB].begin(),
        districtToPrecincts[districtB].end()
    );
    districtToPrecincts[districtB].clear();

    if (treeAlgorithm == SpanningTreeAlgorithm::MST) {
        minSpanningTree(districtA, [](int precinct1, int precinct2){
            return rand();
        }, 0);
    } else if (treeAlgorithm == SpanningTreeAlgorithm::WILSON) {
        wilsonTree(districtA);
    }
    int rootPrecinct = districtToPrecincts[districtA][0];
    calculatePopulations(rootPrecinct);

    // Calcuate mst splitting point
    int totalPopulation = populationCache[rootPrecinct];
    int splitPrecinct = rootPrecinct;
    for (int precinct : districtToPrecincts[districtA]) {
        int currentDif = populationCache[splitPrecinct]*2 - totalPopulation;
        int newDif = populationCache[precinct]*2 - totalPopulation;
        if (abs(newDif) < abs(currentDif)) {
            splitPrecinct = precinct;
        }
    }
    double maxPopulation = totalPopulation * (0.5+tolerance);
    double minPopulation = totalPopulation * (0.5-tolerance);
    double actualPopulation = populationCache[splitPrecinct];
    if (populationCache[splitPrecinct] > maxPopulation || populationCache[splitPrecinct] < minPopulation) {
        districtToPrecincts[districtA] = tempA;
        districtToPrecincts[districtB] = tempB;
        for (int precinct : tempA) {
            precinctToDistrict[precinct] = districtA;
        }
        for (int precinct : tempB) {
            precinctToDistrict[precinct] = districtB;
        }
        return false;
    }

    // Rebuild districts from tree
    districtToPrecincts[districtA].clear();
    dfsRebuild(districtA, rootPrecinct, splitPrecinct);
    dfsRebuild(districtB, splitPrecinct);
    return true;
}


// Calculates the minimum spanning tree of a particular district
void Partition::minSpanningTree(int district, std::function<int(int, int)> getEdgeWeight, int startingPrecinctIdx) {
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
    int startingPrecinct = districtToPrecincts[district][startingPrecinctIdx];
    keyCache[startingPrecinct] = 0;
    pq.push(make_pair(keyCache[startingPrecinct], startingPrecinct));
    while (!pq.empty()) {
        int precinct = pq.top().second;
        pq.pop();
        if (visitedCache[precinct])
            continue;
        visitedCache[precinct] = true;
        
        for (unsigned i = 0; i < edges[precinct].size(); i++) {
            int neighbor = edges[precinct][i];
            // Check if neighbor is in correct district and unvisited
            if (precinctToDistrict[neighbor] == district && !visitedCache[neighbor]) {
                // Since the weight of every edge is only read once during the execution
                // of Prim's algorithm, choosing a random weight for the edge on the spot
                // has the exact same effect as assigning random weights to the graph
                // prior to running Prim's algorithm.
                int weight = getEdgeWeight(precinct, neighbor);
                if (weight < keyCache[neighbor]) {
                    keyCache[neighbor] = weight;
                    parentCache[neighbor] = precinct;
                    pq.push(make_pair(weight, neighbor));
                }
            }
        }
    }

    // Construct treeCache from parentCache
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

void Partition::wilsonTree(int district) {
    for (int precinct : districtToPrecincts[district]) {
        visitedCache[precinct] = false;
        parentCache[precinct] = -1;
        treeCache[precinct].clear();
    }
    const vector<vector<int>>& edges = graph->getEdges();
    int root = districtToPrecincts[district][0];
    visitedCache[root] = true;
    for (int pathStart : districtToPrecincts[district]) {
        if (visitedCache[pathStart]) {
            continue;
        }
        int precinct = pathStart;
        while (!visitedCache[precinct]) {
            std::vector<int> neighbors;
            for (int adj : edges[precinct]) {
                if (precinctToDistrict[adj] == district) {
                    neighbors.push_back(adj);
                }
            }
            int neighbor = neighbors[rand() % neighbors.size()];
            parentCache[precinct] = neighbor;
            precinct = neighbor;
        }
        precinct = pathStart;
        while (!visitedCache[precinct]) {
            visitedCache[precinct] = true;
            precinct = parentCache[precinct];
        }
    }

    // Construct treeCache from parentCache
    for (int precinct : districtToPrecincts[district]) {
        if (precinct == root) {
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
    return populationCache[precinct];
}

void Partition::dfsRebuild(int district, int precinct) {
    districtToPrecincts[district].push_back(precinct);
    precinctToDistrict[precinct] = district;
    for (int child : treeCache[precinct]) {
        dfsRebuild(district, child);
    }
}

void Partition::dfsRebuild(int district, int precinct, int exclude) {
    districtToPrecincts[district].push_back(precinct);
    precinctToDistrict[precinct] = district;
    for (int child : treeCache[precinct]) if (child != exclude) {
        dfsRebuild(district, child, exclude);
    }
}

float Partition::getMeanMedian() {

    // calculate % votes
    // democrat 0, republican 1
    // first is democratic, second is republican
    vector<float> percentageVotes;
    for (int district = 0; district < numDistricts; district++) {
        vector<int> precincts = districtToPrecincts[district];

        // votes per district
        int democraticVotes = 0, republicanVotes = 0;
        for (auto& precinctId: precincts) {
            Precinct precinct = graph->getPrecincts()[precinctId];
            democraticVotes += precinct.democraticVotes;
            republicanVotes += precinct.republicanVotes;
        }
    
        int totalVotes = democraticVotes + republicanVotes;
        percentageVotes.push_back(((float) republicanVotes)/totalVotes);
    }

    // calculate median
    sort(percentageVotes.begin(), percentageVotes.end());

    float median = 0;
    // even
    if(percentageVotes.size() % 2 == 0) {
        median = 0.5 * (
            percentageVotes[percentageVotes.size()/2 - 1] 
            + percentageVotes[percentageVotes.size()/2]
        );
    // odd
    } else {
        median = percentageVotes[percentageVotes.size() / 2];
    }

    float total = 0;
    for(auto& percentageVote: percentageVotes) {
        total += percentageVote;
    }
    float mean = total/percentageVotes.size();

    return median - mean;
}

int Partition::getPopulation(int district) {
    int population = 0;
    for (int precinct : districtToPrecincts[district]) {
        population += graph->getPrecincts()[precinct].population;
    }
    return population;
}

vector<vector<int>>& Partition::getTreeCache() {
    return treeCache;
}

vector<int>& Partition::getDistrictPrecincts(int district) {
    return districtToPrecincts[district];
}

int Partition::getPrecinctDistrict(int precinct) {
    return precinctToDistrict[precinct];
}