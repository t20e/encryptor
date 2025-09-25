#include <iostream>

#include "utils/console_styles.h"
#include "utils/string_utils.h"

using std::cout;
using namespace console_styles;

int renderMainMenu()
{ // returns the number of lines printed
    cout << BOLD << UNDERLINED_CYAN << "\nDrag & drop a file to encrypt or decrypt here" << RESET << "\n\n";
    return 4;
}

// This function prints the menu and the error if any and returns the total lines it prints
// Error message can not be more than 2 lines
int renderAlgorithmSelectionMenu(const std::string &errMsg = "")
{
    int linePrintedCount = 0;

    cout << "\nWhat algorithm do you want to use?\n\n"
         << "Enter:\n"
         << "'1' for Caesar Cipher\n"
         << "'2' for XOR cipher\n"
         << "'3' for Advanced Encryption Standard (AES)\n"
         << "'b' to go back\n\n";
    linePrintedCount = 9;

    if (!errMsg.empty())
    {
        cout << RED << errMsg << RESET;
        return linePrintedCount += 2;
    }
    return linePrintedCount;
}