#include "../include/StateMinimization.h"

void StateMinimization::readKiss()
{
    std::string lineData;
    std::ifstream inKissFile;
    inKissFile.open(fileName);
    std::vector<ProductTerm> implicants;

    while (getline(inKissFile, lineData))
    {
        std::string command;
        std::stringstream ss;

        ss >> command;

        if (command == ".start_kiss")
        {
            // continue;
        }
        else if (command == ".end_kiss")
        {
            break;
        }

        else if (command == ".i")
        {
            ss >> kissFileData.input;
        }

        else if (command == ".o")
        {
            ss >> kissFileData.output;
        }

        else if (command == ".p")
        {
            ss >> kissFileData.p;
        }

        else if (command == ".s")
        {
            ss >> kissFileData.r;
        }

        else if (command == ".r")
        {
            ss >> kissFileData.r;
        }

        else
        {
            std::string in;
            char current;
            char next;
            int out;
            ss >> in >> current >> next >> out;
            implicants.push_back(ProductTerm(in, current, next, out));
            kissFileData.productTerms = implicants;
        }
    }
}

void StateMinimization::buildTable()
{
    char lastState = kissFileData.productTerms[0].currentState;

    for (size_t i = 0; i < kissFileData.s; i++)
    {
        std::vector<ProductTerm> state;
        for (size_t j = 0; j < (kissFileData.p / kissFileData.s); j++)
        {
            ProductTerm productTerm;
            productTerm = kissFileData.productTerms[i * kissFileData.s + j];
            state.push_back(productTerm);
        }
        table.push_back(state);
    }
}

void StateMinimization::buildImplicantTable()
{
    std::vector<std::vector<implicantTableDataPair>>
        implicantTable(kissFileData.s,
                       std::vector<implicantTableDataPair>(
                           kissFileData.s,
                           implicantTableDataPair(false,
                                                  {'X'})));

    for (size_t i = 0; i < kissFileData.s; i++)
    {
        for (size_t j = 0; j < kissFileData.s; j++)
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
                for (size_t k = 1; k < kissFileData.p / kissFileData.s; k++)
                {
                    if (k % 2 == 0)
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
}

std::pair<int, int> StateMinimization::isAllCompatibility()
{
    int row = -1, column = -1;

    for (size_t i = 0; i < implicantTable.size(); i++)
    {
        for (size_t j = 0; j < implicantTable[i].size(); j++)
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

void StateMinimization::compatibilityCheck()
{
    std::pair<int, int> isCompate = isAllCompatibility();
    while (isCompate.first != -1 && isCompate.second != -1)
    {
        implicantTable[isCompate.first][isCompate.second].minimize = false;
        isCompate = isAllCompatibility();
    }
}

void StateMinimization::mergeAllTable(int mergeIndex, char replaceChar)
{
    for (size_t a = 0; a < implicantTable.size(); a++)
    {
        for (size_t b = 0; b < implicantTable[a].size(); b++)
        {
            if (a == mergeIndex || b == mergeIndex)
            {
                implicantTable[a][b].minimize = false;
            }
            for (size_t x = 0; x < implicantTable[a][b].nextStates.size(); x++)
            {
                if (implicantTable[a][b].nextStates[x] == (char)('a' + mergeIndex))
                {
                    implicantTable[a][b].nextStates[x] = replaceChar;
                }
            }
        }
    }

    table.erase(table.begin() + mergeIndex);
}

void StateMinimization::stateMerge()
{
    for (size_t i = 0; i < implicantTable.size(); i++)
    {
        for (size_t j = 0; j < implicantTable[i].size(); j++)
        {
            if (implicantTable[i][j].minimize)
            {
                mergeAllTable(i, (char)(j + 'a'));
            }
        }
    }
}

void StateMinimization::outputKiss()
{
    std::ofstream outKiss;
    outKiss.open(outKissName);

    outKiss << ".start_kiss" << std::endl;
    outKiss << ".i " << kissFileData.input << std::endl;
    outKiss << ".o " << kissFileData.output << std::endl;
    outKiss << ".p " << table[0].size() * table.size() << std::endl;
    outKiss << ".s " << table.size() << std::endl;
    outKiss << ".r " << kissFileData.r << std::endl;
    for (size_t i = 0; i < table.size(); i++)
    {
        for (size_t j = 0; j < table[i].size(); j++)
        {
            outKiss << j << " "
                    << table[i][j].currentState << " "
                    << table[i][j].nextState << " "
                    << table[i][j].output << std::endl;
        }
    }
    outKiss << ".end_kiss" << std::endl;
}

void StateMinimization::outputDot()
{
    std::ofstream outDot;
    outDot.open(outDotName);
    std::vector<char> label;

    outDot << "digraph STG {" << std::endl
           << "\trankdir=LR;" << std::endl
           << std::endl;

    outDot << "\tINIT [shape=point];" << std::endl;
    for (size_t i = 0; i < table.size(); i++)
    {
        for (size_t j = 0; j < table[i].size(); j++)
        {
            if (std::find(label.begin(), label.end(),
                          table[i][j].currentState) != label.end())
            {
                label.push_back(table[i][j].currentState);
            }
            if (std::find(label.begin(), label.end(),
                          table[i][j].nextState) != label.end())
            {
                label.push_back(table[i][j].nextState);
            }
        }
    }

    for (size_t i = 0; i < label.size(); i++)
    {
        outDot << label[i] << " "
               << "[label=" << '"' << label[i] << '"' << "];" << std::endl;
    }

    outDot << std::endl
           << "INIT -> " << kissFileData.r << ";" << std::endl;

    for (size_t i = 0; i < table.size(); i++)
    {
        std::map<char, std::vector<std::pair<std::string, int>>> stateMap;
        for (size_t j = 0; j < table[i].size(); j++)
        {
            std::pair<std::string, int> outPair(table[i][j].input,
                                                table[i][j].output);

            stateMap[table[i][j].currentState].push_back(outPair);
        }
        for (const auto &pair : stateMap)
        {
            outDot << table[i][0].currentState << " -> "
                   << pair.first << " [label=" << '"';
            for (size_t k = 0; k < pair.second.size(); k++)
            {
                if (k != 0)
                    outDot << ", ";
                outDot << pair.second[k].first << '/' << pair.second[k].second;
            }
            outDot << '"' << "];" << std::endl;
        }
    }
    outDot << '}';
}

void StateMinimization::outTable()
{
    for (auto &productTerms : table)
    {
        for (auto &productTerm : productTerms)
        {
            std::cout << productTerm.input << " "
                      << productTerm.currentState << " "
                      << productTerm.nextState << " "
                      << productTerm.output << std::endl;
        }
    }
}

void StateMinimization::outImpTable()
{
    for (auto &implicantTableDataPair : implicantTable)
    {
        for (auto &imp : implicantTableDataPair)
        {
            std::cout << imp.minimize << " ";
            for (auto &state : imp.nextStates)
            {
                std::cout << state << " ";
            }
            std::cout << std::endl;
        }
    }
}

void StateMinimization::outKiss()
{
    std::cout << kissFileData.input << std::endl
              << kissFileData.output << std::endl
              << kissFileData.p << std::endl
              << kissFileData.s << std::endl
              << kissFileData.r << std::endl;
}

StateMinimization::StateMinimization(std::string const &name,
                                     std::string const &kissName,
                                     std::string const &dotName)
{
    fileName = name;
    outKissName = kissName;
    outDotName = dotName;
    readKiss();
    outKiss();
    // buildTable();
    // outTable();
    // buildImplicantTable();
    // outImpTable();
    // compatibilityCheck();
    // stateMerge();
    // outputKiss();
    // outputDot();
}