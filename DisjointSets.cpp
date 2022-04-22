#include "DisjointSets.h"
#include <iostream>

DisjointSets::DisjointSets(int size) {
    parents.resize(size, -1);
    numSets = size;
}

int DisjointSets::find(int elem) {
    int res = elem;
    // Go up the tree
    while (parents[res] >= 0) {
        res = parents[res];
    }
    // Go up again but reassign all the parents to directly point to the top node
    while (parents[elem] >= 0) {
        parents[elem] = res;
        elem = parents[elem];
    }
    return res;
}

void DisjointSets::join(int a, int b) {
    // Get representative elements
    a = find(a);
    b = find(b);

    // Ignore if they're in the same set
    if (a == b) {
        return;
    }

    // Reassign parent of smaller tree to merge
    if (-parents[a] < -parents[b]) {
        parents[b] += parents[a];
        parents[a] = b;
    } else {
        parents[a] += parents[b];
        parents[b] = a;
    }
    numSets--;
}

int DisjointSets::getNumSets() {
    return numSets;
}