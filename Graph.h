#include <vector>

using std::vector;
class Graph {
    vector<vector<int>> edges;
    vector<Precinct> precincts;
    // nodes are uniquely identified by id/number

    // write BFS to a file
    // to find node distance
    void BFS(int start, int end);

};