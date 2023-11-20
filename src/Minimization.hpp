#include "Kiss.hpp"
#include "Table.hpp"

vector<vector<ProductTerm>> buildTable(kissData &kiss)
{
    char lastState = kiss.productTerms[0].currentState;
    vector<vector<ProductTerm>> states;

    for (size_t i = 0; i < kiss.s; i++)
    {
        vector<ProductTerm> state;
        for (size_t j = 0; j < (kiss.p / kiss.s); j++)
        {
            ProductTerm productTerm;
            productTerm = kiss.productTerms[i * kiss.s + j];
            state.push_back(productTerm);
        }
        states.push_back(state);
    }

    return states;
}

vector<vector<implicantTableDataPair>>
buildImplicantTable(vector<vector<ProductTerm>> const &table, kissData &kiss)
{
    vector<vector<implicantTableDataPair>>
        implicantTable(kiss.s,
                       vector<implicantTableDataPair>(
                           kiss.s,
                           implicantTableDataPair(false,
                                                  {'X', 'X', 'X', 'X'})));

    for (size_t i = 0; i < kiss.s; i++)
    {
        for (size_t j = 0; j < kiss.s; j++)
        {
            if (j >= i ||
                (table[i][0].output != table[i][1].output) ||
                (table[j][0].output != table[j][1].output))
            {
                continue;
            }
            else
            {
                implicantTable[i][j].minimize = true;
                implicantTable[i][j].nextStates[0] = table[i][0].nextState;
                implicantTable[i][j].nextStates[1] = table[j][0].nextState;
                implicantTable[i][j].nextStates[2] = table[i][1].nextState;
                implicantTable[i][j].nextStates[3] = table[j][0].nextState;
            }
        }
    }

    return implicantTable;
}
