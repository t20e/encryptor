#pragma once

#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

namespace fs = std::filesystem; // name space for file-system

/**
 * @brief Save byte data stored in a vector to a file.
 * 
 * @param buffer The vector byte data.
 * @param filename The name of the file, including the extension.
 * @param saveToDirectory  The path to the directory to save the file to.
 */
void saveFile(const std::vector<unsigned char> &buffer, std::string &filename, fs::path &saveToDirectory);


/**
 * @brief Read and return a file's content as a sequence of bytes, works well for alphabetic character, irregular characters, and raw binary data.
 * 
 * @param filepath Path to the selected file
 * @return std::vector<unsigned char> 
 */
std::vector<unsigned char> read_file_contents(const fs::path &filepath);


/**
 * @brief Converts a vector of bytes into human-readable hexadecimal string, used to display encrypted files.
 * 
 * @param buffer 
 * @return std::string 
 */
std::string to_hex_string(const std::vector<unsigned char> &buffer);
