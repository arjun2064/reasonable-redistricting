#include <iostream>
#include "Precinct.h"
#include "Graph.h"
#include "Partition.h"

using std::cout;
using std::endl;

int main(int argc, char *argv[]) {
    srand(time(NULL));

    Graph graph("cdata.txt");
    Partition partition(&graph, 10);
    partition.recombination(0, 1);
}