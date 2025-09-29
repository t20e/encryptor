#pragma once

#include <string>

std::string extract_filename(const std::string filePath);
// void clearConsoleLines(const short numLines);

/**
 * @brief Shorten the strings of path if they are too long.
 *
 * @param path The full path to a file
 * @return std::string
 */
std::string shorten_path(const std::string &path);
