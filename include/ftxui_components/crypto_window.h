#pragma once

#include <ftxui/component/component.hpp> // User input, Radiobox

using namespace ftxui;

/**
 * @brief The Cryptography window.
 */
class CryptoWindow : public ComponentBase
{
  private:
	// State variables
	bool is_Decrypting = false;
	std::string selected_file_path = "";
	std::string selected_folder_save_path = "";

	// For the radiobox
	Component radioboxComp;
	int selectedRadioBtn = 0; // For the radioboxComp

	// For text input
	Component textInputComp;
	std::string textInput = "";

	//   The text that goes above the input, letting the user know what the text input should be
	std::string algoHeader = "";

	// Action button
	Component actionButton;
	std::string buttonLabel;

	std::vector<std::string> algo_radio = {
		"None",
		"Caesar cipher",
		"XOR Cipher",
		"AES (Advanced Encryption Standard)",
	};

	// Whether this window component has visible child components. Nothing is shown if no file is selected.
	bool show_content = false;

	/**
	 * @brief # TODO
	 *
	 * @param window
	 */
	void onEncryptDecryptBtn();

  public:
	CryptoWindow();

	// --- Getters & Setters ---
	// int getSelectedAlgorithm() const
	// {
	// 	return selected;
	// }
	// const std::string getTextInput() const
	// {
	// 	return textInput;
	// }
	const std::string getSelectedFilePath() const
	{
		return this->selected_file_path;
	}

	// Setters
	void setSelectedFilePath(std::string selected_file_path)
	{
		this->selected_file_path = selected_file_path;
	}

	void setShowContent(bool is_visible)
	{
		this->show_content = is_visible;
	}
	void setIsDecrypting(bool is_decrypting)
	{
		this->is_Decrypting = is_decrypting;
	}

	/**
	 * @brief Set the state variables to back to default
	 */
	void setToDefault()
	{
		// State variables
		is_Decrypting = false;
		selected_file_path = "";
		selected_folder_save_path = "";

		// For the radiobox
		selectedRadioBtn = 0; // For the radioboxComp

		// For text input
		textInput = "";
		algoHeader = "";
		show_content = false;
	}
};