#include <string>
#include <vector>

class implicantData
{
public:
    string input;
    char currentState;
    char nextState;
    int output;

    implicantData() {}
    implicantData(string i, char c, char n, int o)
        : input(i), currentState(c), nextState(n), output(o) {}
};

struct kissData
{
    int input;
    int output;
    int p;
    int s;
    char r;
    std::vector<implicantData> implicants;
};

class tableDataPair
{
public:
    char nextState;
    int out;

    tableDataPair() {}
    tableDataPair(char n, int o) : nextState(n), out(o) {}
};