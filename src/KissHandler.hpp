#include <fstream>
#include <sstream>
#include "Kiss.hpp"

void readKiss(std::vector<implicantData> &implicants,
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
            string in;
            char current;
            char next;
            int out;
            ss >> in >> current >> next >> out;
            implicants.push_back(implicantData(in, current, next, out));
            kiss.implicants = implicants;
        }
    }
}