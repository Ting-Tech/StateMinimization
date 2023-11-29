#pragma once
#include <string>
#include <vector>

class ProductTerm
{
public:
    std::string input;
    std::string currentState;
    std::string nextState;
    int output;

    ProductTerm() {}
    ProductTerm(std::string i, std::string c, std::string n, int o)
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
