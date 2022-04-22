#include <iostream>
#include "Precinct.h"
#include "Graph.h"

using std::cout;
using std::endl;

int main(int argc, char *argv[]) {
    Graph graph("cdata.txt");
    cout << graph.getPrecincts()[0].population << endl;
    cout << graph.getPrecincts()[0].democraticVotes << endl;
    cout << graph.getPrecincts()[0].republicanVotes << endl;
    cout << graph.getEdges()[0][0] << endl;
}