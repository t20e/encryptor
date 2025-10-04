#pragma once

#include <filesystem> // to read system files and folders
#include <string>
#include <unordered_map>
#include <vector>

namespace fs = std::filesystem;

// Type alias
using algoMap = std::unordered_map<std::string, std::string>;


/**
 * @brief Store global variables to be passed around.
 * 
 */
struct AppModel
{
		std::vector<unsigned char> selectedFileContents; // The contents of the selected file to encrypt or decrypt.
		std::vector<unsigned char> outFileContents; // The contents after encrypting or decrypting.

		bool isDecrypting = false;
		bool showFilePreviews = false;

		// Define a header to add the to beginning of an encrypted file, to identify it.
		//      Header -> ENCRYPTOR_1_
		//                 the   _(int)_ is the algo identifier,
		//                _1_ -> caesar_cipher, _2_ -> XOR, _3_ -> AES, etc...
		const std::string FILE_HEADER_ = "ENCRYPTOR";

		fs::path selected_file_path; // reset with fs::path{}
		fs::path selected_folder_to_save_to_path_;

		std::unordered_map<int, algoMap> algosDict{
				{
						0,
						{
								{"name", "None"},
								{"prompt", "Select an algorithm"},
						},
				},
				{
						1,
						{
								{"name", "Caesar Cipher"},
								{"prompt", "Algo: How much to shift the characters in the file?"},
						},
				},
				{
						2,
						{
								{"name", "XOR Cipher"},
								{"prompt", "Algo: //TODO"},
						},
				},
				{
						3,
						{
								{"name", "AES (Advanced Encryption Standard)"},
								{"prompt", "Algo: // TODO"},
						},
				},
		};
};
