#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <iostream>

std::string extract_filename(const std::string filePath);
std::string trim_trailing_whitespace(const std::string filePath);
std::string removeDoubleQuotes(std::string filePath);
void clearLine();

#endif 