#include "utils/string_utils.h"

std::string shorten_path(const std::string &path)
{
	short maxlength = 64; // Max length a path string can be.
	if (path.length() <= maxlength)
	{
		return path;
	}
	std::string placeholder = "/././.";
	short placeholderLength = placeholder.length();
	short suffixLength = maxlength - placeholderLength;

	// Determine how long the ending of the path should be
	// short suffixLength = remainingLength / 2;

	// Grab the ending part of the string
	std::string suffix = path.substr(path.length() - suffixLength);
	return placeholder + suffix;
}


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

 

// /**
//  * @brief Clear console lines
//  *
//  * @param numLines the number of lines to clear.
//  * @return * void
//  * @note
//  */
// void clearConsoleLines(const short numLines)
// {
//     // for (int i = 0; i < numLines; i++)
//     // {
//     //     cout << MOVE_UP_ONE_LINE;
//     //     cout << CLEAR_LINE;
//     // }
// }
