#pragma once

#include <filesystem> // to read system files and folders
#include <ftxui/component/component.hpp> // User input, Radiobox

using namespace ftxui;
namespace fs = std::filesystem; // namespace for filesystem

// Type alias
using algoMap = std::unordered_map<std::string, std::string>;

/**
 * @brief The Cryptography window.
 */
class CryptoWindow : public ComponentBase
{
	private:
		// State variables
		bool is_Decrypting_ = false;
		std::string outFileContents_ = ""; // The contents after encrypting or decrypting.
		std::string inFileContents_ = ""; // The contents of the selected file to encrypt or decrypt.

		fs::path selected_file_path_;

		// For the radioboxinFileContents_
		Component radioboxComp_;
		int selectedRadioBtn_ = 0; // For the radioboxComp_

		// Algorithm hash input
		Component hashInput_Comp_;
		std::string hashInput_ = "";

		// New file name for the user to create.
		Component newFileNameInput_Comp_;
		std::string newFileNameInput_ = "";
		fs::path selected_folder_to_save_path_;

		// Action button
		Component actionButton_;

		// Whether this window component has visible child components. Nothing is shown if no file is selected.
		bool show_content_ = false;

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

		/**
         * @brief Set the up algo radio array (algo_radio_), with algorithms names from (algosDict)
         * 
         * @return std::vector<std::string> 
         */
		std::vector<std::string> setup_algo_radio();


		/**
	 * @brief # TODO
	 *
	 * @param window
	 */
		void onSaveBtn();

		// Callback Logic member functions. Pass to the FileBrowser class.
		/**
	 * @brief Callback function when user selects a file from the file browser window.
	 *
	 * @param path Full path to that file.
	 */
		void onFileSelected(const std::string &path);

	public:
		CryptoWindow();

		// --- Getters & Setters ---
		const fs::path getSelectedFilePath()
		{
			return this->selected_file_path_;
		}

		// Setters
		void setSelectedFilePath(fs::path selected_file_path)
		{
			this->selected_file_path_ = selected_file_path;
		}
		void setSelectedFolderPath(fs::path selected_folder_path)
		{
			this->selected_folder_to_save_path_ = selected_folder_path;
		}

		void setShowContent(bool is_visible)
		{
			this->show_content_ = is_visible;
		}
		void setIsDecrypting(bool is_decrypting)
		{
			this->is_Decrypting_ = is_decrypting;
		}

		/**
     * @brief Reset state variables back to default.
     * 
     */
		void reset()
		{
			is_Decrypting_ = false;
			outFileContents_ = "";
			inFileContents_ = "";
			selected_file_path_ = fs::path{};
			selectedRadioBtn_ = 0; // For the radioboxComp_
			// Algorithm hash input
			hashInput_ = "";
			// New file name for the user to create.
			newFileNameInput_ = "";
			selected_folder_to_save_path_ = fs::path{};
		}
};
