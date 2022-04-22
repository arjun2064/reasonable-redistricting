#include <iostream>
#include "Precinct.h"
#include "Graph.h"
#include "Partition.h"

using std::cout;
using std::endl;

int main(int argc, char *argv[]) {
    srand(time(NULL));

    Graph graph("cdata.txt");
    cout << graph.getPrecincts()[0].population << endl;
    cout << graph.getPrecincts()[0].democraticVotes << endl;
    cout << graph.getPrecincts()[0].republicanVotes << endl;
    cout << graph.getEdges()[0][0] << endl;
    cout << graph.BFS(0,13) << endl;
    cout << graph.BFS(0,2876) << endl;
    cout << graph.BFS(0,8000) << endl;
    Partition partition(&graph, 10);
}