#pragma once
#include <fstream>
#include <sstream>
#include "./Kiss.h"
#include "./Table.h"

class StateMinimization
{
private:
    std::pair<int, int> isAllCompatibility();

    void mergeAllTable(int mergeIndex, char replaceChar);

public:
    std::string fileName;
    kissData kissFileData;
    std::vector<std::vector<ProductTerm>> kiss;
    std::vector<std::vector<ProductTerm>> table;
    std::vector<std::vector<implicantTableDataPair>> implicantTable;

    StateMinimization(std::string const &name);

    void readKiss();

    std::vector<std::vector<ProductTerm>> buildTable();

    std::vector<std::vector<implicantTableDataPair>> buildImplicantTable();

    void compatibilityCheck();

    void stateMerge();
};
