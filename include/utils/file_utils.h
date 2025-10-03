#pragma once

#include <fstream>
#include <string>
#include <vector>

namespace fs = std::filesystem; // name space for file-system

// int writeFile(const std::string &text, AlgoIdentifier id);
// inline int saveFile(std::string filename, std::string &content, AlgoIdentifier &id)
// {
// 	// Create and open a file stream
// 	std::ofstream file(filename);
// 	// Write to file
// 	file << content;
// 	// close stream
// 	file.close();
// 	return 0; // TODO Learn how to deal with errors in cpp
// }


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
