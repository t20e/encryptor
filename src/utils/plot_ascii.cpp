#include <iostream>
#include <fstream>

#include "utils/plot_ascii.h"
#include "utils/console_styles.h"

using std::cout;
using namespace console_styles; // instead of having to call it as  console_styles::MOVE_UP_ONE_LINE, etc...

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