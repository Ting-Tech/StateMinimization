#include "Kiss.hpp"

vector<vector<tableDataPair>> buildTable(kissData &kiss)
{
    char lastState = kiss.implicants[0].currentState;
    vector<tableDataPair> state;
    vector<vector<tableDataPair>> states;

    for (auto implicantsIndex : kiss.implicants)
    {
        if (implicantsIndex.currentState != lastState)
        {
            states.push_back(state);
            vector<tableDataPair> state;
        }
        else
        {
            tableDataPair statePair;
            statePair.nextState = implicantsIndex.currentState;
            statePair.out = implicantsIndex.output;
            state.push_back(statePair);
        }
    }

    return states;
}

vector<vector<implicantTableDataPair>>
buildImplicantTable(vector<vector<tableDataPair>> const &table)
{
}
