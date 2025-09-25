#include <iostream>
#include <filesystem>

// Link my header files, notice the ""
#include "utils/plot_ascii.h"
#include "utils/console_styles.h"
#include "utils/string_utils.h"
#include "algos/AES.h"

using std::cout;
using namespace console_styles; // instead of having to call it as console_styles::MOVE_UP_ONE_LINE, just call MOVE_UP_ONE_LINE

// Use a state machine that utilizes a single main while loop.
enum State
{
    MAIN_MENU,
    SELECT_ALGORITHM,
    ENCRYPTING,
    DECRYPTING,
    QUIT
};

int main()
{
    State currState = MAIN_MENU;

    plot_ASC_II();
    // TODO add paragraph here like about what this program does, maybe use the same one from the README.md file

    cout << "\nEnter 'q' to quit!\n";

    while (currState != QUIT)
    {
        std::string filePath;
        char operation;
        std::string key;
        std::string filename;
        int linesToClear = 0;

        switch (currState)
        {
        case MAIN_MENU:
            linesToClear = renderMainMenu();
            // TODO: there has to be a way to do , or type its file path from current working directory or root -> without having to type it in, maybe using `cd`
            // === Validate file
            std::getline(std::cin, filePath); // input
            clearConsoleLines(linesToClear);

            if (filePath == "q")
            {
                currState = QUIT;
                break;
            }
            // TODO better file name input validation needed, example: just the enter key is pressed or number 2 key

            filePath = removeDoubleQuotes(filePath);
            // Trim any trailing whitespaces
            filePath = trim_trailing_whitespace(filePath);

            filename = extract_filename(filePath);
            cout << "\nFile: " << CYAN << filename << RESET << "\n";

            if (!std::filesystem::exists(filePath))
            {
                cout << "\nFile does not exist, try again!";
                break;
            }

            // Check if the file is encrypted, so we can decrypt it
            if (filePath.size() > 4 && filePath.substr(filePath.size() - 4) == ".enc")
            {
                currState = DECRYPTING;
                break;
            }

            currState = SELECT_ALGORITHM;
            break;

        case SELECT_ALGORITHM:
        { // using a local block {} for a switch case allows us to define errorMsg in it.
            // Holds errors so we can pass them to the menu display functions
            std::string errorMsg = "";

            while (true)
            { // Loop until the user give a valid input or backs out.
                int linesPrinted = renderAlgorithmSelectionMenu(errorMsg);

                // Get user input
                std::cin >> operation;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard character from the input buffer.
                //// std::cin.clear(); // Clear the input buffer.

                // Valid the input
                if (operation == '1' || operation == '2' || operation == '3' || operation == 'b' || operation == 'q')
                { // valid input
                    clearConsoleLines(linesPrinted);
                    break;
                }

                errorMsg = "\nInvalid option, try again!";
                clearConsoleLines(linesPrinted);
            }

            if (operation == '1')
            {
                cout << "1";
                // Caesar cipher
            }
            else if (operation == '2')
            {
                cout << "2";
                // XOR cipher
            }
            else if (operation == '3')
            {
                cout << "3";
                // AES
            }
            else if (operation == 'b')
            {
                clearConsoleLines(3); // clear the loaded filename line
                currState = MAIN_MENU;
            }
            else if (operation == 'q')
            {
                cout << "QUITTING";
                currState = QUIT;
            }
            break;
        }
        case ENCRYPTING:
            cout << "THERE";
            break;
        case DECRYPTING:
            break;
        case QUIT: // Suppresses not all enum cases warning.
            break;
        }
    }

    // TODO ask them if they want to continue decrypting or encrypting other files

    cout << "\n"
         << std::string(90, '-') << "\n\n";

    return 0;
}