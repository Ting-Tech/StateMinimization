#include "Kiss.hpp"
#include "Table.hpp"

std::vector<std::vector<ProductTerm>> buildTable(kissData &kiss)
{
    char lastState = kiss.productTerms[0].currentState;
    std::vector<std::vector<ProductTerm>> states;

    for (size_t i = 0; i < kiss.s; i++)
    {
        std::vector<ProductTerm> state;
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

std::vector<std::vector<implicantTableDataPair>>
buildImplicantTable(
    std::vector<std::vector<ProductTerm>> const &table, kissData &kiss)
{
    std::vector<std::vector<implicantTableDataPair>>
        implicantTable(kiss.s,
                       std::vector<implicantTableDataPair>(
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

std::pair<int, int>
isAllCompatibility(
    std::vector<std::vector<implicantTableDataPair>> &implicantTable)
{
    int row = -1, column = -1;

    for (size_t i = 0; i < implicantTable.size(); i++)
    {
        for (size_t j = 0; j < implicantTable.size(); j++)
        {
            if (!implicantTable[i][j].minimize)
                continue;

            int state1 = implicantTable[i][j].nextStates[0] - 'a';
            int state2 = implicantTable[i][j].nextStates[1] - 'a';
            int state3 = implicantTable[i][j].nextStates[2] - 'a';
            int state4 = implicantTable[i][j].nextStates[3] - 'a';

            if (implicantTable[state1][state2].minimize ||
                implicantTable[state3][state4].minimize)
            {
                row = i;
                column = j;
                return std::make_pair(row, column);
            }
        }
    }

    return std::make_pair(row, column);
}

void compatibilityCheck(
    std::vector<std::vector<implicantTableDataPair>> &implicantTable)
{
    pair<int, int> isCompate = isAllCompatibility(implicantTable);
    while (isCompate.first != -1 && isCompate.second != -1)
    {
        implicantTable[isCompate.first][isCompate.second].minimize = false;
        isCompate = isAllCompatibility(implicantTable);
    }
}

void stateMerge(std::vector<std::vector<ProductTerm>> &table,
                std::vector<std::vector<implicantTableDataPair>> &implicantTable)
{
}