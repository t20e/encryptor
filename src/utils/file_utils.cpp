#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

#include "utils/file_utils.h"


namespace fs = std::filesystem; // name space for file-system

void saveFile(const std::vector<unsigned char> &buffer, std::string &filename, fs::path &saveToDirectory, const std::optional<std::string> &FILE_HEADER_IDENTIFIER)
{
	fs::path savePath = saveToDirectory / filename;

	// Create and open a file stream in binary mode
	std::ofstream file(savePath, std::ios::binary);

	if (!file.is_open()) {
		std::cerr << "Error: could not open file for writing: " << filename << std::endl; //TODO add error handling
		return;
	}

	if (FILE_HEADER_IDENTIFIER) { // If encrypting, write the header to the file.
		// Get a reference to the header string inside the optional by using `FILE_HEADER_IDENTIFIER->` or you could also use FILE_HEADER_IDENTIFIER.value()
		file.write(FILE_HEADER_IDENTIFIER->c_str(), FILE_HEADER_IDENTIFIER->size());
	}

	// Write the raw vector data to the file.
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
		// TODO  error handling//
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

	// TODO error handling
	// Error: failed to read the file
	return {};
}


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


bool checkIfEncryptedFile(const std::vector<unsigned char> &buffer, std::string FILE_HEADER_IDENTIFIER)
{
	// Make sure buffer is long enough, to contain the header
	if (buffer.size() < FILE_HEADER_IDENTIFIER.length()) {
		return false;
	}

	// Check if the id is in the header, by comparing both sequences byte-by-byte.
	return std::equal(FILE_HEADER_IDENTIFIER.begin(), FILE_HEADER_IDENTIFIER.end(), buffer.begin());
}

std::vector<unsigned char> removeHeaderIdentifier(
	const std::vector<unsigned char> &buffer,
	const std::string &FILE_HEADER_IDENTIFIER)
{
	// Create a new vector that copies the data after the header
	return std::vector<unsigned char>(buffer.begin() + FILE_HEADER_IDENTIFIER.length(), buffer.end());
}
