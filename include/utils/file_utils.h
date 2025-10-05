#pragma once

#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

namespace fs = std::filesystem; // name space for file-system

/**
 * @brief Save the vector of bytes data to a file.
 * 
 * @param buffer The vector byte data.
 * @param filename The name of the file, including the extension.
 * @param saveToDirectory  The path to the directory to save the file to.
 * @param FILE_HEADER_IDENTIFIER The encryption header identity to add to the beginning of the file, to not save the header use empty string ("").
 */
void saveFile(const std::vector<unsigned char> &buffer, std::string &filename, fs::path &saveToDirectory, const std::optional<std::string> &FILE_HEADER_IDENTIFIER);


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

/**
 * @brief Check if a file has the FILE_HEADER_IDENTIFIER in its header.
 * 
 * @param buffer The vector containing the bytes of the file
 * @return true If its an encrypted file.
 * @return false If its a regular file.
 */
bool checkIfEncryptedFile(const std::vector<unsigned char> &buffer, std::string FILE_HEADER_IDENTIFIER);


/**
 * @brief Remove the file header identifier from a vector of bytes.
 * 
 * @param buffer THe input vector.
 * @param FILE_HEADER_IDENTIFIER The header string.
 * @return std::vector<unsigned char> A new vector without the header.
 */
std::vector<unsigned char> removeHeaderIdentifier(
		const std::vector<unsigned char> &buffer,
		const std::string &FILE_HEADER_IDENTIFIER);