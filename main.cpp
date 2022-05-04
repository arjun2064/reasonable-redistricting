#include <iostream>
#include "Precinct.h"
#include "Graph.h"
#include "Partition.h"
#include <fstream>
#include <string>

using std::cout;
using std::endl;

void recombinationWithOutput(Partition& partition, std::string filename) {

    // dry run to reach equilibrium convergence
    for (int i = 0; i < 20000; ++i) {
        partition.recombination();
        if (i > 0 && i % 1000 == 0) {
            cout << "dry run finished " << i << endl;
        }
    }

    std::ofstream fout("./output/mean-median-mst.txt");
    for (int i = 0; i < 20000; ++i) {
        partition.recombination();
        fout << partition.getMeanMedian() << endl;
        if (i > 0 && i % 1000 == 0) {
            cout << "saved run finished " << i << endl;
        }
    }
}

int main(int argc, char *argv[]) {
    srand(time(NULL));

    Graph graph("cdata.txt");

    // MST
    Partition mstPartition(&graph, 10, Partition::SpanningTreeAlgorithm::MST);
    cout << "MST Recom" << endl;
    recombinationWithOutput(mstPartition, "./output/mean-median-mst.txt");
    cout << "MST Recom output to ./output/mean-median-mst.txt" << endl;
    cout << endl;

    // Wilson
    Partition wilsonPartition(&graph, 10, Partition::SpanningTreeAlgorithm::WILSON);
    cout << "Wilson Recom" << endl;
    recombinationWithOutput(wilsonPartition, "./output/mean-median-wilson.txt");
    cout << "MST Recom output to ./output/mean-median-wilson.txt" << endl;
    cout << endl;
}