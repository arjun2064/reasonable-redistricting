#include <vector>
#include "Precinct.h"

using std::string;
using std::vector;
class Graph {
    public:
        Graph();
        Graph(string filename);
        void loadFromFile(string filename);

    private:
        int numPrecincts = 0;

        /*
            nodes are uniquely identified by id/number
            precincts, being vertexes, connected by edges
        */
        vector<Precinct> precincts;

        // edges between precincts and connecting them
        vector<vector<int>> edges;

        // write BFS to a file
        // to find node distance
        void BFS(int start, int end);
};