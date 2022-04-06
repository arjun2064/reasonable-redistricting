#include <vector>
#include "Precinct.h"

using std::vector;
class Graph {
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
  void BFSComponent(int vertex);

};