#pragma once
#include <vector>

class tableDataPair
{
public:
    char nextState;
    int out;

    tableDataPair() {}
    tableDataPair(char n, int o) : nextState(n), out(o) {}
};

class implicantTableDataPair
{
public:
    bool minimize;
    std::vector<std::vector<std::string>> nextStates;

    implicantTableDataPair() {}
    implicantTableDataPair(bool m, std::vector<std::vector<std::string>> n)
        : minimize(m), nextStates(n) {}
};