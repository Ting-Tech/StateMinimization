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
    std::vector<char> nextStates;

    implicantTableDataPair() {}
    implicantTableDataPair(bool m, std::vector<char> n)
        : minimize(m), nextStates(n) {}
};