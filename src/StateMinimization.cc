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
        std::stringstream ss(lineData);

        ss >> command;

        if (command == ".start_kiss")
        {
            continue;
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
            ss >> kissFileData.s;
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
            int out = 0;
            in = command;
            ss >> current >> next >> out;
            implicants.push_back(ProductTerm(in, current, next, out));
            kissFileData.productTerms = implicants;
        }
    }
}

void StateMinimization::buildTable()
{
    for (size_t i = 0; i < kissFileData.s; i++)
    {
        std::vector<ProductTerm> tableRaw;
        for (size_t j = 0; j < kissFileData.p / kissFileData.s; j++)
        {
            ProductTerm p;

            p.currentState =
                kissFileData.productTerms
                    [((kissFileData.p / kissFileData.s) * i) + j]
                        .currentState;
            p.input =
                kissFileData.productTerms
                    [((kissFileData.p / kissFileData.s) * i) + j]
                        .input;
            p.nextState =
                kissFileData.productTerms
                    [((kissFileData.p / kissFileData.s) * i) + j]
                        .nextState;
            p.output =
                kissFileData.productTerms
                    [((kissFileData.p / kissFileData.s) * i) + j]
                        .output;

            tableRaw.push_back(p);
        }
        table.push_back(tableRaw);
    }
}

bool isNextStateSame(const std::vector<std::vector<ProductTerm>> &t,
                     int index1, int index2)
{
    for (size_t i = 1; i < t[index1].size(); i++)
    {
        char last = t[index1][0].nextState;
        if (t[index1][i].output != t[index2][i].output)
        {
            return true;
        }
    }
    return false;
}

void StateMinimization::buildImplicantTable()
{
    implicantTable.resize(kissFileData.s,
                          std::vector<implicantTableDataPair>(
                              kissFileData.s,
                              implicantTableDataPair(false, {{}, {}})));

    for (size_t i = 0; i < kissFileData.s; i++)
    {
        for (size_t j = 0; j < kissFileData.s; j++)
        {
            if (isNextStateSame(table, i, j))
            {
                continue;
            }
            if (j < i)
            {
                implicantTable[i][j].nextStates[0].resize(table[i].size(), 'a');
                implicantTable[i][j].nextStates[1].resize(table[i].size(), 'a');
                for (size_t x = 0; x < table[i].size(); x++)
                {
                    implicantTable[i][j].nextStates[x][0] =
                        table[i][x].nextState;
                    implicantTable[i][j].nextStates[x][1] =
                        table[j][x].nextState;
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

            int state1 = implicantTable[i][j].nextStates[0][0] - 'a';
            int state2 = implicantTable[i][j].nextStates[0][1] - 'a';
            int state3 = implicantTable[i][j].nextStates[1][0] - 'a';
            int state4 = implicantTable[i][j].nextStates[1][1] - 'a';

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
    // for (size_t a = 0; a < implicantTable.size(); a++)
    // {
    //     for (size_t b = 0; b < implicantTable[a].size(); b++)
    //     {
    //         if (a == mergeIndex || b == mergeIndex)
    //         {
    //             implicantTable[a][b].minimize = false;
    //         }
    //         for (size_t x = 0; x < implicantTable[a][b].nextStates.size(); x++)
    //         {
    //             if (implicantTable[a][b].nextStates[x] == (char)('a' + mergeIndex))
    //             {
    //                 implicantTable[a][b].nextStates[x] = replaceChar;
    //             }
    //         }
    //     }
    // }

    // table.erase(table.begin() + mergeIndex);
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
    std::cout << "Table:" << std::endl;
    for (auto &productTerms : table)
    {
        for (auto &productTerm : productTerms)
        {
            std::cout << productTerm.input << " "
                      << productTerm.currentState << " "
                      << productTerm.nextState << " "
                      << productTerm.output << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "Table end" << std::endl
              << std::endl;
}

void StateMinimization::outImpTable()
{
    std::cout << "Imp table:" << std::endl;
    for (auto &implicantTableDataPair : implicantTable)
    {
        for (auto &imp : implicantTableDataPair)
        {
            std::cout << imp.minimize << " ";
            for (auto &states : imp.nextStates)
            {
                for (size_t i = 0; i < states.size(); i++)
                {
                    std::cout << states[i];
                }
                std::cout << '-';
            }
            std::cout << '|';
        }
        std::cout << std::endl;
    }
    std::cout << "Imp table end" << std::endl
              << std::endl;
}

void StateMinimization::outKiss()
{
    std::cout << "kiss.start" << std::endl
              << kissFileData.input << std::endl
              << kissFileData.output << std::endl
              << kissFileData.p << std::endl
              << kissFileData.s << std::endl
              << kissFileData.r << std::endl;

    for (size_t i = 0; i < kissFileData.productTerms.size(); i++)
    {
        std::cout << kissFileData.productTerms[i].input << " "
                  << kissFileData.productTerms[i].currentState << " "
                  << kissFileData.productTerms[i].nextState << " "
                  << kissFileData.productTerms[i].output << " "
                  << std::endl;
    }

    std::cout << "kiss.end" << std::endl
              << std::endl;
}

StateMinimization::StateMinimization(std::string const name,
                                     std::string const kissName,
                                     std::string const dotName)
    : fileName(name), outKissName(kissName), outDotName(dotName)

{
    std::cout << &fileName << std::endl;
    readKiss();
    outKiss();
    buildTable();
    outTable();
    buildImplicantTable();
    outImpTable();
    compatibilityCheck();
    outImpTable();
    // stateMerge();
    // outputKiss();
    // outputDot();
}