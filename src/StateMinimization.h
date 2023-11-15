#include <string>
#include <vector>
#include <StateMinimization.cc>

class StateMinimization
{
private:
    struct
    {
        std::vector<int> input;
        std::vector<int> output;
        std::vector<char> currentState;
        std::vector<char> nextState;
    } implicantTable;

public:
    void setImplicantTable();
};
