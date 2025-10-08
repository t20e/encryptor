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
	std::ofstream file;

	// Set exception mask to through file errors.
	file.exceptions(std::ios::failbit | std::ios::badbit);

	// Open the newly created file in binary mode, if error it will throw and must be catched
	file.open(savePath, std::ios::binary);

	if (FILE_HEADER_IDENTIFIER) { // If encrypting, write the header to the file.
		// Get a reference to the header string inside the optional by using `FILE_HEADER_IDENTIFIER->` or you could also use FILE_HEADER_IDENTIFIER.value()
		file.write(FILE_HEADER_IDENTIFIER->c_str(), FILE_HEADER_IDENTIFIER->size());
	}

	// Write the raw vector data to the file. The write() needs a pointer to the data (const char*), and the number of bytes to write. Will throw if it fails, must be caught
	file.write(
		reinterpret_cast<const char *>(buffer.data()),
		buffer.size());

	file.close();
}


std::vector<unsigned char> read_file_contents(const fs::path &filepath)
{
	std::ifstream file;
	file.exceptions(std::ios::failbit | std::ios::badbit); // Tell the stream to throw an exception on failure.

	// Open file in binary mode and position the get pointer at the end.
	file.open(filepath, std::ios::binary | std::ios::ate);

	// Get the size of the file.
	std::streamsize size = file.tellg();
	file.seekg(0, std::ios::beg); // Reset the pointer to the beginning of the file.

	// Create a vector of the right size and store the whole file into it.
	std::vector<unsigned char> buffer(size);

	file.read(reinterpret_cast<char *>(buffer.data()), size);
	return buffer;
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


std::string to_hex_preview(const std::vector<unsigned char> &buffer, size_t max_bytes)
{
	std::stringstream ss;

	ss << std::hex << std::setfill('0');
	// std::hex sets number format to hexadecimal, any value will now be converted to hex, e.g., `255` becomes `ff`
	// std::setfill('0') Set the padding character to 0, `A` becomes `0A`

	size_t bytes_to_show = std::min(buffer.size(), max_bytes);

	// Iterate over each byte of the vector of the part that we have to show.
	for (size_t i = 0; i < bytes_to_show; i++) {
		if (i % 16 == 0) { // At the start of a new line every 16 bytes print the address offset.
			if (i > 0) {
				ss << "\n";
			}
			// Add the address offset for the new line.
            ss<< "0x"<<std::setw(8) << i << ": ";

		}
        ss << std::setw(2) << static_cast<int>(buffer[i]) << " ";
	}
	if (buffer.size() > max_bytes) {
        ss << "\n\n... (file truncated, " << buffer.size() - bytes_to_show << " more bytes not shown)...";
	}
	return ss.str();
}


bool is_likely_binary(const std::vector<unsigned char> &buffer)
{
	size_t check_size = std::min(buffer.size(), (size_t) 1024);

	for (size_t i = 0; i < check_size; i++) {
		if (buffer[i] == '\0') {
			// A null byte is a strong indication of a binary file.
			return true;
		}
		// check for other non-printable characters
		if (!std::isprint(buffer[i]) && !std::isspace(buffer[i])) {
			return true;
		}
	}
	// If no non-text characters were found, then the file is likely a text file.
	return false;
}
