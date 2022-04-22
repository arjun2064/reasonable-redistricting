#pragma once
#include <vector>

/**
 * Represents a family of disjoint sets,
 * where each element has an integer index
 */
class DisjointSets {
    public:
        DisjointSets(int size);
        int find (int elem);
        void join (int a, int b);
        int getNumSets();

    private:
        // current number of sets
        int numSets;
        // The container of the up-trees
        std::vector<int> parents;
};