#include "Kiss.hpp"
#include <fstream>
#include <sstream>

void readKiss(std::vector<ProductTerm> &implicants,
              kissData &kiss,
              std::string const &fileName)
{
    std::string lineData;
    std::ifstream inKissFile;
    inKissFile.open(fileName);

    while (getline(inKissFile, lineData))
    {
        std::string command;
        std::stringstream ss;

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
            ss >> kiss.input;
        }

        else if (command == ".o")
        {
            ss >> kiss.output;
        }

        else if (command == ".p")
        {
            ss >> kiss.p;
        }

        else if (command == ".s")
        {
            ss >> kiss.r;
        }

        else if (command == ".r")
        {
            ss >> kiss.r;
        }

        else
        {
            std::string in;
            char current;
            char next;
            int out;
            ss >> in >> current >> next >> out;
            implicants.push_back(ProductTerm(in, current, next, out));
            kiss.productTerms = implicants;
        }
    }
}