#include "utils/string_utils.h"

std::string shorten_path(const std::string &path)
{
	short maxlength = 42; // Max length a path string can be.
	if (path.length() <= maxlength) {
		return path;
	}

	short maxFilenameLength = 32;
	std::string placeholder = "/././";
	std::string dotPlaceholder = "...";

	// --- Suffix
	short placeholderLength = placeholder.length();
	short suffixLength = maxlength - placeholderLength;

	// Grab the ending part of the full path
	std::string suffix = path.substr(path.length() - suffixLength);
	short first_slash_pos = suffix.find("/");
	if (first_slash_pos != std::string::npos) {
		// Keep only the part after the first slash
		suffix = suffix.substr(first_slash_pos + 1);
	}

	// --- Handle large filenames
	short last_slash_idx = suffix.find_last_of("/\\"); // The index of the last "/" 0 based-indexing

	std::string dir_path = "";
	std::string filename = suffix;

	if (last_slash_idx != std::string::npos) {
		// Extract teh directory path portion
		dir_path = suffix.substr(0, last_slash_idx + 1);
		filename = suffix.substr(last_slash_idx + 1);
	}

	if (filename.length() > maxFilenameLength) { // shorten the filename
		short dotPlaceholderLength = dotPlaceholder.length();
		// Split the filename in halves
		short split_len = (maxFilenameLength - dotPlaceholderLength) / 2;

		// Truncate the first halve
		std::string front_part = filename.substr(0, split_len);
		std::string back_part = filename.substr(filename.length() - split_len);

		// Combine
		filename = front_part + dotPlaceholder + back_part;
	}

	return placeholder + dir_path + filename;
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
