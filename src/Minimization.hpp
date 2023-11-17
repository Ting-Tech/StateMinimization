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
    vector<vector<implicantTableDataPair>> implicantTable[kiss.s][kiss.s];

    for (size_t i = 1; i < kiss.s; i++)
    {
        for (size_t j = 0; j < kiss.s; j++)
        {
            // implicantTable[i][j] = ;
        }
    }

    // return implicantTable;
}
