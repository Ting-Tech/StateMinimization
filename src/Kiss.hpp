#include <string>
#include <vector>

class ProductTerm
{
public:
    string input;
    char currentState;
    char nextState;
    int output;

    ProductTerm() {}
    ProductTerm(string i, char c, char n, int o)
        : input(i), currentState(c), nextState(n), output(o) {}
};

struct kissData
{
    int input;
    int output;
    int p;
    int s;
    char r;
    std::vector<ProductTerm> productTerms;
};
