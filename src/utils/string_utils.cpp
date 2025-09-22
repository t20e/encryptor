#include <iostream>
#include "string_utils.h"

using std::cout;

std::string extract_filename(const std::string filePath)
{
    // Extract a filename from full string path.
    size_t last_slash_idx = filePath.find_last_of("/\\"); // The index of the last "/" 0 based-indexing
    if (std::string::npos != last_slash_idx)
    {
        return filePath.substr(last_slash_idx + 1);
    }
    else
    {
        return filePath;
    }
}

std::string trim_trailing_whitespace(const std::string filePath)
{
    size_t last_char_idx = filePath.find_last_not_of(" \t\n\r");
    if (std::string::npos == last_char_idx)
    {
        return "";
    }
    return filePath.substr(0, last_char_idx + 1);
}

std::string removeDoubleQuotes(std::string filePath)
{
    // For some systems the drag & drop files can contain quotes ex: "C:\Users\Documents..." <- "", so remove them
    if (filePath.length() > 1 && filePath.front() == '"' && filePath.back() == '"')
    {
        filePath = filePath.substr(1, filePath.length() - 2);
        return filePath;
    }
    return filePath;
}

void clearLine()
{
    cout << "\033[2K\r"; // ANSI escape code to clear the current line
}