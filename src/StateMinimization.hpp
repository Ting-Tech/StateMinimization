#include <string>
#include <vector>

class implicantData
{
public:
    int input;
    char currentState;
    char nextState;
    int output;

    implicantData(int i, char c, char n, int o)
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
