
#include "ftxui/dom/elements.hpp"
#include <array>
#include <string>

#include "ftxui/component/component_options.hpp" // for InputOption
#include "ftxui/dom/elements.hpp"
#include <ftxui/component/component.hpp> // User input, Radiobox
#include <ftxui/dom/elements.hpp>		 // for filler, text, hbox, vbox, vflex

// My headers
#include "algos/algo_identifier.h"
#include "ftxui_components/crypto_window.h"
#include "utils/string_utils.h"

using namespace ftxui;

/**
 * @brief Get the Display texts for whether the user is encrypting or decrypting, and for what algorithm is being used
 *
 * @param is_Decrypting
 * @param selected The index of the selected algorithm.
 * @return std:: Array of texts. First index being for the text header and the second for the input text placeholder.
 */
std::array<std::string, 2> getDisplayTexts(bool &is_Decrypting, int &selected)
{
	std::string placeHolderText = "";
	std::string textHeader = "";

	if (is_Decrypting)
	{
		textHeader = "Select the same algorithm that was used to encrypt the file";
	}
	else
	{
		textHeader = "Select encryption algorithm:";
	}

	switch (selected)
	{
		case static_cast<int>(AlgoIdentifier::caesar_cipher):
			// caesar cipher
			placeHolderText = "How much to shift the characters in the file?";
			break;
		case static_cast<int>(AlgoIdentifier::XOR_cipher):
			placeHolderText = ""; // TODO add XOR placeholder
			// caesar cipher
			break;
		case static_cast<int>(AlgoIdentifier::AES):
			placeHolderText = ""; // TODO addd AES placeholder
			// caesar cipher
			break;
		default: // invalid or None
			break;
	}

	return {textHeader, placeHolderText};
}

// Class constructor
CryptoWindow::CryptoWindow()
{
	// Radiobox
	// Event that runs when the user selects an algo from the radiobox list.
	auto on_radiobox_algo_change = [this] {
		// Clear the text in the input field
		this->textInput = "";
	};
	// Create an object to hold the above action.
	RadioboxOption radio_option;
	radio_option.on_change = on_radiobox_algo_change;

	// Initialize the components
	radioboxComp = Radiobox(&this->algo_radio, &this->selectedRadioBtn, radio_option);
	textInputComp = Input(&this->textInput, "Input...");

	this->actionButton = Button("Save", [this] { this->onEncryptDecryptBtn(); });

	// Group interactive components in a Container, which manages the events and focus between its children, then we can
	// put them in an Element like vbox.
	auto interactiveContainer = Container::Vertical({
		radioboxComp,
		textInputComp,
		actionButton,
	});

	// This lambda Renderer function re-runs everytime the UI needs to be redrawn, for each frame. ALL DYNAMIC LOGIC GOES INSIDE IT.
	auto content = Renderer(interactiveContainer, [this] {
		if (!this->show_content)
		// if (true) // TODO Del this line and uncomment above line
		{
			return text("Select a file from the file browser window!") | center;
		}

		// Get the texts for the different cases
		auto texts = getDisplayTexts(this->is_Decrypting, this->selectedRadioBtn);
		std::string &textHeader = texts[0];

		// Update the placeholder text
		this->algoHeader = texts[1];

		Elements elem = {
			text(is_Decrypting ? "---Decrypting---" : "---Encrypting---") | center,
			separatorEmpty(),
			hbox({
				// Display the path to the file clicked.
				text("Selected file: ") | bold,
				text((selected_file_path != "") ? shorten_path(selected_file_path) : "No file selected") | color(Color::CyanLight),
			}) | center,
			separatorEmpty(),
			text(textHeader),
			separatorEmpty(),
			hbox({
				text("    "), // indent
				radioboxComp->Render(),
			}),
			separatorEmpty(),
		};

		// Check if an algo has been selected to display the text input field.
		if (this->selectedRadioBtn != 0)
		{
			elem.push_back(text(this->algoHeader));
			elem.push_back(hbox({
				separatorEmpty(),
				separatorEmpty(),
				textInputComp->Render() | border | bgcolor(Color::White) | color(Color::White),
				separatorEmpty(),
				separatorEmpty(),
			}));
			if (!this->textInput.empty())
			{ // if the user has entered text, then prompt to select a folder in the File browser to save the encrypted or decrypted file.
				elem.push_back(vbox({
					separatorEmpty(),
					text(std::format("Select a folder in th File browser window to save the {} file:", this->is_Decrypting ? "Decrypt" : "Encrypt")),
					separatorEmpty(),
					// TODO replace action button to after the folder input
					hbox({
						separatorEmpty() | flex,
						actionButton->Render(),
						separatorEmpty() | flex,
					}),
				}));
			}
		}

		return vbox(elem);
	});

	// Add() builds the component hierarchy
	Add(content);
}

// Class methods
void CryptoWindow::onEncryptDecryptBtn()
{
	if (this->getSelectedFilePath().empty())
	{
		return; // no file selected
	}

	// // Get the state variables from CryptoWindow instance
	// int selected_algo = window.getSelectedAlgorithm();
	// const std::string textInput = window.getTextInput();

	// TODO Encryption and decryption logic here!
}
