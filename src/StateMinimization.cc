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
            std::string current;
            std::string next;
            int out = 0;
            in = command;
            ss >> current >> next >> out;
            implicants.push_back(ProductTerm(in, current, next, out));
            kissFileData.productTerms = implicants;
            if (std::find(symbals.begin(), symbals.end(),
                          current) == symbals.end())
            {
                symbals.push_back(current);
            }
            if (std::find(symbals.begin(), symbals.end(),
                          next) == symbals.end())
            {
                symbals.push_back(next);
            }
        }
    }
    // for (auto &a : symbals)
    // {
    //     std::cout << a << std::endl;
    // }
}

int searchIndex(const std::vector<std::string> &vec, const std::string &target)
{
    for (size_t i = 0; i < vec.size(); ++i)
    {
        if (vec[i] == target)
        {
            return static_cast<int>(i); // Convert size_t to int for the index
        }
    }
    return -1; // Return -1 if the element is not found
}

void StateMinimization::buildTable()
{
    std::vector<std::vector<ProductTerm>> copyTable;
    for (size_t i = 0; i < kissFileData.s; i++)
    {
        std::vector<ProductTerm> tableRow;
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

            tableRow.push_back(p);
        }
        copyTable.push_back(tableRow);
    }
    for (size_t a = 0; a < symbals.size(); a++)
    {
        for (size_t b = 0; b < copyTable.size(); b++)
        {
            if (symbals[a] == copyTable[b][0].currentState)
            {
                table.push_back(copyTable[b]);
            }
        }
    }
}

bool StateMinimization::isNextStateSame(int index1, int index2)
{
    for (size_t h = 0; h < table[index1].size(); h++)
    {
        if (table[index1][h].output != table[index2][h].output)
        {
            return false;
        }
    }
    return true;
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
            if (isNextStateSame(i, j))
            {
                if (j < i)
                {
                    implicantTable[i][j].minimize = true;

                    implicantTable[i][j].nextStates[0].resize(table[i].size());
                    implicantTable[i][j].nextStates[1].resize(table[i].size());
                    for (size_t x = 0; x < table[i].size(); x++)
                    {
                        if (searchIndex(symbals, table[i][x].nextState) <
                            searchIndex(symbals, table[j][x].nextState))
                        {
                            implicantTable[i][j].nextStates[0][x] =
                                table[j][x].nextState;
                            implicantTable[i][j].nextStates[1][x] =
                                table[i][x].nextState;
                        }
                        else
                        {
                            implicantTable[i][j].nextStates[0][x] =
                                table[i][x].nextState;
                            implicantTable[i][j].nextStates[1][x] =
                                table[j][x].nextState;
                        }
                    }
                }
            }
        }
    }
}

std::pair<int, int> StateMinimization::isAllCompatibility()
{
    for (size_t i = 0; i < implicantTable.size(); i++)
    {
        for (size_t j = 0; j < implicantTable[i].size(); j++)
        {
            if (implicantTable[i][j].minimize)
            {
                for (size_t k = 0;
                     k < implicantTable[i][j].nextStates[0].size(); k++)
                {
                    int index1 =
                        searchIndex(symbals, implicantTable[i][j].nextStates[0][k]);
                    int index2 =
                        searchIndex(symbals, implicantTable[i][j].nextStates[1][k]);

                    if ((index1 != 0) && (index2 < index1))
                    {
                        if (!implicantTable[index1][index2].minimize)
                        {
                            return std::make_pair(i, j);
                        }
                    }
                }
            }
        }
    }

    return std::make_pair(-1, -1);
}

void StateMinimization::compatibilityCheck()
{
    for (;;)
    {
        std::pair<int, int> isCompate = isAllCompatibility();
        if (isCompate.first == -1 || isCompate.second == -1)
        {
            break;
        }
        implicantTable[isCompate.first][isCompate.second].minimize = false;
    }
}

void StateMinimization::mergeAllTable(int mergeIndex, std::string replaceChar)
{
    for (size_t x = 0; x < implicantTable.size(); x++)
    {
        for (size_t y = 0; y < implicantTable[x].size(); y++)
        {
            if (x == mergeIndex || y == mergeIndex)
            {
                implicantTable[x][y].minimize = false;
            }
            for (size_t j = 0; j < implicantTable[x][y].nextStates.size(); j++)
            {
                for (size_t k = 0;
                     k < implicantTable[x][y].nextStates[j].size(); k++)
                {
                    if (implicantTable[x][y].nextStates[j][k] == symbals[mergeIndex])
                    {
                        implicantTable[x][y].nextStates[j][k] = replaceChar;
                    }
                }
            }
        }
    }
}

void StateMinimization::stateMerge()
{
    std::map<int, int> eraseIndex;
    for (size_t a = 0; a < implicantTable.size(); a++)
    {
        for (size_t b = 0; b < implicantTable[a].size(); b++)
        {
            if (implicantTable[a][b].minimize)
            {
                implicantTable[a][b].minimize = false;
                for (size_t i = 0;
                     i < implicantTable[a][b].nextStates[0].size(); i++)
                {
                    mergeAllTable(a, symbals[b]);
                    eraseIndex[a] = b;
                }
            }
        }
    }

    std::vector<std::pair<int, int>> vecPairs(eraseIndex.begin(), eraseIndex.end());
    std::sort(vecPairs.begin(), vecPairs.end(),
              std::greater<std::pair<int, int>>());
    for (auto &index : vecPairs)
    {
        table.erase(table.begin() + index.first);

        std::string target = symbals[index.first];
        std::string replace = symbals[index.second];

        for (size_t i = 0; i < table.size(); i++)
        {
            for (size_t j = 0; j < table[i].size(); j++)
            {
                if (table[i][j].nextState == target)
                {
                    table[i][j].nextState = replace;
                }
            }
        }
    }
}

std::string decimalToBinary(const std::string &decimalString)
{
    // 將輸入的十進制字串轉換為整數
    int decimalNumber = std::stoi(decimalString);

    if (decimalNumber == 0)
    {
        return "0"; // Special case for zero
    }

    std::string binaryResult = "";

    while (decimalNumber > 0)
    {
        // 取得餘數（0 或 1）並轉換為字元
        char remainder = '0' + (decimalNumber % 2);

        // 將餘數插入到字串的最前面
        binaryResult = remainder + binaryResult;

        // 進行短除法，將十進制數字除以 2
        decimalNumber /= 2;
    }

    return binaryResult;
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
            outKiss << table[i][j].input << " "
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
    std::vector<std::string> label;

    outDot << "digraph STG {" << std::endl
           << "\trankdir=LR;" << std::endl
           << std::endl;

    outDot << "\tINIT [shape=point];" << std::endl;
    for (size_t i = 0; i < table.size(); i++)
    {
        for (size_t j = 0; j < table[i].size(); j++)
        {
            if (std::find(label.begin(), label.end(),
                          table[i][j].currentState) == label.end())
            {
                label.push_back(table[i][j].currentState);
            }
            if (std::find(label.begin(), label.end(),
                          table[i][j].nextState) == label.end())
            {
                label.push_back(table[i][j].nextState);
            }
        }
    }

    for (size_t i = 0; i < label.size(); i++)
    {
        outDot << "\t" << label[i] << " "
               << "[label=" << '"' << label[i] << '"' << "];" << std::endl;
    }

    outDot << std::endl
           << "\t"
           << "INIT -> " << kissFileData.r << ";" << std::endl;

    for (size_t i = 0; i < table.size(); i++)
    {
        std::map<std::pair<std::string, std::string>,
                 std::vector<std::pair<std::string, int>>>
            stateMap;
        for (size_t j = 0; j < table[i].size(); j++)
        {
            std::pair<std::string, int> outPair(table[i][j].input,
                                                table[i][j].output);

            stateMap[std::make_pair(table[i][j].currentState,
                                    table[i][j].nextState)]
                .push_back(outPair);
        }
        for (const auto &pair : stateMap)
        {
            outDot << "\t" << pair.first.first << "->"
                   << pair.first.second << " [label=" << '"';
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
            for (size_t i = 0; i < imp.nextStates[0].size(); i++)
            {
                for (size_t j = 0; j < imp.nextStates.size(); j++)
                {
                    std::cout << imp.nextStates[j][i] << '-';
                }
                std::cout << '_';
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
    readKiss();
    outKiss();
    buildTable();
    outTable();
    buildImplicantTable();
    outImpTable();
    compatibilityCheck();
    outImpTable();
    stateMerge();
    outTable();
    outImpTable();
    outputKiss();
    outputDot();
}