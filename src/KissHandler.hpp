#include <fstream>
#include <sstream>
#include "StateMinimization.hpp"

void readKiss(std::vector<implicantData> &implicants,
              kissData &kissData,
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
            continue;
        }

        else if (command == ".i")
        {
            ss >> kissData.input;
        }

        else if (command == ".o")
        {
            ss >> kissData.output;
        }

        else if (command == ".p")
        {
            ss >> kissData.p;
        }

        else if (command == ".s")
        {
            ss >> kissData.r;
        }

        else if (command == ".r")
        {
            ss >> kissData.r;
        }

        else
        {
            int in;
            char current;
            char next;
            int out;
            ss >> in >> current >> next >> out;
            implicants.push_back(implicantData(in, current, next, out));
        }
    }
}