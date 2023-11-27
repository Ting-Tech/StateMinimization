#include <iostream>
#include <string>
#include <vector>
#include "../include/StateMinimization.h"

using namespace std;

int main(int argc, char *argv[])
{
    if (argc == 4)
    {
        StateMinimization stateminimize(argv[1], argv[2], argv[3]);
    }
    else
    {
        cout << "please key in the output file name!" << endl;
    }
}