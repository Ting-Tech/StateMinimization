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
                                                  {'X'})));

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
                int outputIndex1 = 1, outputIndex2 = 0;
                implicantTable[i][j].nextStates[0] = table[i][0].nextState;
                for (size_t i = 1; i < kiss.p / kiss.s; i++)
                {
                    if (i % 2 == 0)
                    {
                        implicantTable[i][j].nextStates.push_back(
                            table[i][outputIndex1].nextState);
                        outputIndex1++;
                    }
                    else
                    {
                        implicantTable[i][j].nextStates.push_back(
                            table[j][outputIndex2].nextState);
                        outputIndex2++;
                    }
                }
            }
        }
    }

    return implicantTable;
}
