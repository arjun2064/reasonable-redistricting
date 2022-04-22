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
    randomJoinInitialize();

}

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
    districts.resize(numDistricts);
    for (int i=0; i<numPrecincts; i++) {
        int rootIdx = dsets.find(i);
        if (!idxMap.count(rootIdx)) {
            idxMap[rootIdx] = districtIdx++;
        }
        districts[idxMap[rootIdx]].push_back(i);
    }
}