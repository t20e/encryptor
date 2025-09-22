#include <iostream>
#include <fstream>

#include "ascii.h"
#include "globals.h"

using std::cout;

int plot_ASC_II()
{

    // Plot start ASCII art
    cout << "\n\n\n"
         << std::string(90, '-') << std::endl
         << "\n\n";

    // Open file stream
    std::ifstream asciiFile("misc/ascii.txt");

    if (!asciiFile.is_open())
    {
        std::cerr << RED << "Error: Unable to open file";
        return 1;
    }

    std::string line;

    // Plot the file, line by line.
    while (std::getline(asciiFile, line))
    {
        cout << line << std::endl;
    }
    
    // close file stream
    asciiFile.close();

         
    return 0;
}