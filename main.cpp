#include <iostream>
#include "Precinct.h"
#include "Graph.h"
#include "Partition.h"
#include <fstream>

using std::cout;
using std::endl;

int main(int argc, char *argv[]) {
    srand(time(NULL));

    Graph graph("cdata.txt");
    Partition partition(&graph, 10, Partition::SpanningTreeAlgorithm::MST);

    // partition.recombination();

    // dry run to reach equilibrium convergence
    for (int i = 0; i < 20000; ++i) {
        partition.recombination();
        if (i > 0 && i % 1000 == 0) {
            cout << "dry run finished " << i << endl;
        }
    }

    // std::ofstream fout("mean-median.txt");
    // for (int i = 0; i < 20000; ++i) {
    //     partition.recombination();
    //     fout << partition.getMeanMedian() << endl;
    // }
}