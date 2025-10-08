#pragma once

#include <fstream>

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


/**
 * @brief Generates a hexadecimal string representation (hex dump) of a byte vector, specifically for safely previewing the contents of binray files. The output is formatted into rows of 1 bytes. each prefixed with its address offset.
 * 
 * @param buffer A vector of bytes.
 * @param max_bytes The maximum number of bytes from the buffer to process for the previewer.
 * @return std::string A formatted string containing the hex dump.
 */
std::string to_hex_preview(const std::vector<unsigned char> &buffer, size_t max_bytes = 4096);


/**
 * @brief Determine if a data buffer is likely binary. Checks for null terminators and non-printable characters which are common in binary files and rare in plain text files, however it is not a guaranteed check!
 * 
 * @param buffer Vector of bytes to check.
 * @return true 
 * @return false 
 */
bool is_likely_binary(const std::vector<unsigned char> &buffer);