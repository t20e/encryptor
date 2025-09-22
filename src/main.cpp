#include <iostream>
#include <filesystem>
#include <cstdlib>

// link header files
#include "ascii.h"
#include "globals.h"
#include "string_utils.h"

using std::cout;

int main()
{
    plot_ASC_II();
    // TODO add paragraph here like about what this program does

    cout << "\nEnter 'q' to quit!\n";

    while (true)
    {
        std::string filePath;
        char operation;
        std::string key;

        // === User file input
        cout << BOLD << UNDERLINED_CYAN << "\nDrag & drop the file to encrypt or decrypt here" << RESET << "\n\n";
        // TODO: there has to be a way to do , or type its file path from current working directory or root -> without having to type it in, maybe using `cd`

        // === Validate file
        std::getline(std::cin, filePath); // input
        cout << MOVE_UP_ONE_LINE;
        clearLine(); // Clear one line up

        if (filePath == "q")
            goto end_program;

        filePath = removeDoubleQuotes(filePath);

        // Trim any trailing whitespaces
        filePath = trim_trailing_whitespace(filePath);

        std::string filename = extract_filename(filePath);
        cout << "File: " << PURPLE << filename << RESET << "\n";

        if (!std::filesystem::exists(filePath))
        {
            cout << "\nFile does not exist, try again!";
            continue;
        }

        // === Operation
        while (true)
        {
            cout << "\nEnter 'e' to encrypt, 'd' to decrypt or 'b' to go back: ";
            std::cin >> operation;
            std::cin.clear(); // Clear the input buffer.

            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard character from the input buffer.

            switch (operation)
            {
            case 'e':
                cout << "-E-";
                // TODO encrypt
                break;
            case 'd':
                cout << "-D-";
                // TODO dencrypt
                break;
            case 'b':
                // break;
                goto end_loop;
            case 'q':
                goto end_program;
            default:
                cout << "Invalid option, try again!\n";
                continue;
            }
        }
    end_loop:
        cout << "Exited loop.\n";

        // TODO ask them if they want to continue decrypting or encrypting other files
    }

end_program:

    cout << "\n"
         << std::string(90, '-') << "\n\n";

    return 0;
}