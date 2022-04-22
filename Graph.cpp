#include <vector>
#include <queue>
#include <iostream>
#include <fstream>
#include <climits>

#include "Precinct.h"
#include "Graph.h"

using std::vector;
using std::queue;
using std::cout;
using std::endl;
using std::ifstream;

Graph::Graph() {}

Graph::Graph(string filename) {
    loadFromFile(filename);
}

void Graph::loadFromFile(string filename) {
    ifstream fin(filename);
    fin >> numPrecincts;
    for (int i = 0; i < numPrecincts; i++) {
        Precinct precinct;
        precinct.id = i;
        fin >> precinct.population;
        fin >> precinct.democraticVotes;
        fin >> precinct.republicanVotes;
        int numNeighbors;
        fin >> numNeighbors;
        vector<int> neighbors(numNeighbors);
        for (int j = 0; j < numNeighbors; j++) {
            fin >> neighbors[j];
        }
        precincts.push_back(precinct);
        edges.push_back(neighbors);
    }
}

int Graph::BFS(unsigned start, unsigned end){

    //established temporary vector to keep track of visited nodes
    vector<bool> visited;
    vector<int> distance;
    for(size_t i = 0; i < precincts.size(); i++){
        visited.push_back(false);
        distance.push_back(-1);
    }
    distance[start] = 0;

    queue<int> search;
    search.push(start);

    while(!search.empty()){
        int cur = search.front();

        // otherwise, if not searched, set searched and add everything in connected
        if(!visited[cur]){
            visited[cur] = true;

            for(size_t i = 0; i < edges[cur].size(); i++){
                //if there is an edge between the presincts
                if((unsigned)edges[cur][i] == end){
                    //ctrl + LSquareBracket is <- 1 tab
                    cout << "PRESINCT " << end << " IS " <<  distance[cur] + 1 << " EDGES FROM " << start << "\n";
                    return distance[cur];
                }

                search.push(edges[cur][i]);
                distance[edges[cur][i]] = distance[cur] + 1;
            }
        }
    }
    return -1;
}

const vector<Precinct>& Graph::getPrecincts() {
    return precincts;
}

const vector<vector<int>>& Graph::getEdges() {
    return edges;
}
