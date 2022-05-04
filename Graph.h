#pragma once

#include <vector>
#include "Precinct.h"

using std::string;
using std::vector;
class Graph {
    public:
        Graph();
        Graph(string filename);
        void loadFromFile(string filename);
        const vector<Precinct>& getPrecincts();
        void setPrecincts(vector<Precinct> precincts);
        const vector<vector<int>>& getEdges();
        void setEdges(vector<vector<int>> edges);
        int numPrecincts();
        // write BFS to a file to find node distance
        int BFS(unsigned start, unsigned end);

    private:
        /*
            nodes are uniquely identified by id/number
            precincts, being vertexes, connected by edges
        */
        vector<Precinct> precincts;

        // edges between precincts and connecting them
        vector<vector<int>> edges;
};
