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

    std::ofstream fout(filename);
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

    if (argc != 3) {
        std::cerr << "Please specify BOTH input filename and output prefix" << endl;
        std::cerr << "./recombination <input filename> <output prefix>" << endl;
        std::cerr << "eg ./recombination cdata.txt ./output/mean-median" << endl;
        return 1;
    }

    Graph graph(argv[1]);
    std::string outputPrefix = argv[2];

    // MST
    Partition mstPartition(&graph, 10, Partition::SpanningTreeAlgorithm::MST);
    cout << "MST Recom" << endl;
    std::string mstFilename = outputPrefix + "-mst.txt";
    // std::string mstFilename = "./output/mean-median-mst-pa.txt";
    recombinationWithOutput(mstPartition, mstFilename);
    cout << "MST Recom output to" + mstFilename << endl;
    cout << endl;

    // Wilson
    Partition wilsonPartition(&graph, 10, Partition::SpanningTreeAlgorithm::WILSON);
    cout << "Wilson Recom" << endl;
    std::string wilsonFilename = outputPrefix + "-wilson.txt";
    // std::string wilsonFilename = "./output/mean-median-wilson-pa.txt";
    recombinationWithOutput(wilsonPartition, wilsonFilename);
    cout << "MST Recom output to" + wilsonFilename << endl;
    cout << endl;
}