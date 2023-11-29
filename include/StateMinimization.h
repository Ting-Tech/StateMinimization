#pragma once

#include <map>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iostream>
#include "./Kiss.h"
#include "./Table.h"

class StateMinimization
{
public:
    std::string fileName;
    std::string outKissName;
    std::string outDotName;
    kissData kissFileData;
    std::vector<std::string> symbals;
    std::vector<std::vector<ProductTerm>> kiss;
    std::vector<std::vector<ProductTerm>> table;
    std::vector<std::vector<implicantTableDataPair>> implicantTable;

    void readKiss();
    void buildTable();
    bool isNextStateSame(int index1, int index2);
    void buildImplicantTable();
    void mergeAllTable(int mergeIndex, std::string replaceChar);
    std::pair<int, int> isAllCompatibility();
    void compatibilityCheck();
    void stateMerge();
    void outputKiss();
    void outputDot();
    void outTable();
    void outImpTable();
    void outKiss();
    StateMinimization(std::string const name,
                      std::string const kissName,
                      std::string const dotName);
};
