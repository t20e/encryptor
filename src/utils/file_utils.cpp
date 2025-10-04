#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>


#include "utils/file_utils.h"


namespace fs = std::filesystem; // name space for file-system

void saveFile(const std::vector<unsigned char> &buffer, std::string &filename, fs::path &saveToDirectory)
{

	fs::path savePath = saveToDirectory / filename;


	// Create and open a file stream in binary mode
	std::ofstream file(savePath, std::ios::binary);

	if (!file.is_open()) {
		std::cerr << "Error: could not open file for writing: " << filename << std::endl; //TODO add error handling
		return;
	}

	// Write raw vector data to the file.
	// The write() needs a pointer to the data (const char*), and the number of bytes to write.
	file.write(
			reinterpret_cast<const char *>(buffer.data()),
			buffer.size());

	// Check for writing errors:
	if (file.fail()) {
		std::cerr << "Error: Failed to write all data to file: " << filename << std::endl; //TODO add error handling
		return;
	}

	file.close();
}


std::vector<unsigned char> read_file_contents(const fs::path &filepath)
{
	// Open file in binary mode and position the get pointer at the end.
	std::ifstream file(filepath, std::ios::binary | std::ios::ate);

	if (!file.is_open()) {
		// TODO //
		// File not found, or no permissions, etc!.
		return {};
	}

	// Get the size of the file.
	std::streamsize size = file.tellg();

	// Check for an empty file.
	if (size == 0) {
		return {};
	}

	file.seekg(0, std::ios::beg); // Reset the pointer to the beginning of the file.

	// Create a vector of the right size and store the whole file into it.
	std::vector<unsigned char> buffer(size);
	if (file.read(reinterpret_cast<char *>(buffer.data()), size)) {
		return buffer;
	}

	// TODO
	// Error: failed to read the file
	return {};
}

/**
 * @brief Converts a vector of bytes into human-readable hexadecimal string, used to display encrypted files.
 * 
 * @param buffer 
 * @return std::string 
 */
std::string to_hex_string(const std::vector<unsigned char> &buffer)
{
	std::stringstream ss;
	ss << std::hex << std::setfill('0');

	for (const auto &byte: buffer) {
		// Cast to int to ensure its printed as a number and not a byte character.
		ss << std::setw(2) << static_cast<int>(byte) << " ";
	}
	return ss.str();
}
