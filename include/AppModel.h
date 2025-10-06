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
		const std::string FILE_HEADER_IDENTIFIER = "ENCRYPTOR";

		fs::path selected_file_path; // reset with fs::path{}
		fs::path selected_folder_to_save_to_path;
        bool isSelectingSaveDirectory = false; // Flag for if the user is in the current state of selecting a save directory after encrypting or decrypting a file.
        fs::path directoryToRefresh; // When a user saves a file to an already open directory, we need that directory in the file browser window to reload its children contents so that the new file is shown.


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
								{"prompt", "How much to shift the characters in the file?"},
                                {"informUserHowToDecrypt", "To decrypt later, use the negative of the encryption shift size:"},
                                {"about", ""},
						},
				},
				{
						2,
						{
								{"name", "XOR Cipher"},
								{"prompt", "//TODO"},
                                {"informUserHowToDecrypt", ""},
                                {"about", ""},
						},
				},
				{
						3,
						{
								{"name", "AES (Advanced Encryption Standard)"},
								{"prompt", "// TODO"},
                                {"informUserHowToDecrypt", ""},
                                {"about", ""},
						},
				},
		};
};
