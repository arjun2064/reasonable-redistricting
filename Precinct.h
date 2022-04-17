#pragma once

#include <string>

using std::string;

struct Precinct {
    string name;
    int id;
    int population;
    int republicanVotes;
    int democraticVotes;

    void test();
};