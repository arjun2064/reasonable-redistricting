#pragma once

#include <string>

using std::string;

struct Precinct {
    int id;
    int population;
    int republicanVotes;
    int democraticVotes;

    void test();
};