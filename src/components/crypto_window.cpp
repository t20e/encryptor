
#include <array>
#include <filesystem> // to read system files and folders
#include <format>
#include <string>

#include <ftxui/component/component.hpp> // User input, Radiobox
#include <ftxui/dom/elements.hpp> // for filler, text, hbox, vbox, vflex
#include "ftxui/component/component_options.hpp" // for InputOption
#include "ftxui/dom/elements.hpp"

// My headers
#include "components/crypto_window.h"
#include "utils/string_utils.h"

using namespace ftxui;
// namespace fs = std::filesystem;

// Class constructor
CryptoWindow::CryptoWindow()
{

	// Setup the radio component with the items from algoDict
	std::vector<std::string> algo_radio_ = setup_algo_radio();

	// Radiobox :Event that runs when the user selects an algo from the radiobox list.
	auto on_radiobox_algo_change = [this] {
		this->hashInput_ = ""; // Clear the text in the input field
	};

	// Add input options:
	RadioboxOption radio_option;
	radio_option.on_change = on_radiobox_algo_change;

	InputOption newFolderInputOptions;

	/**
     * @brief A Lambda event to track when user types into newFileNameInput_.
     * 
     */
	newFolderInputOptions.on_change = [this] {
		// --- Add `.enc` extension to the end of filename as user types.
		const std::string filenameEnding = ".enc";
		if (!newFileNameInput_.ends_with(filenameEnding)) {
			newFileNameInput_ += filenameEnding;
		}

		// --- Encrypt or Decrypt a file: ---
		if (is_Decrypting_) {
		}
	};

	// Initialize the components
	radioboxComp_ = Radiobox(algo_radio_, &this->selectedRadioBtn_, radio_option);
	hashInput_Comp_ = Input(&this->hashInput_, "Input...");
	newFileNameInput_Comp_ = Input(&this->newFileNameInput_, "File name...", newFolderInputOptions);

	this->actionButton_ = Button("Save", [this] { this->onSaveBtn(); });

	// Group interactive components in a Container, which manages the events and focus between its children, then we can
	// put them in an Element like vbox.
	auto interactiveContainer = Container::Vertical({
			radioboxComp_,
			hashInput_Comp_,
			newFileNameInput_Comp_,
			actionButton_,
	});

	// This lambda Renderer function re-runs everytime the UI needs to be redrawn, for each frame. ALL DYNAMIC LOGIC GOES INSIDE IT.
	auto content = Renderer(interactiveContainer, [this, interactiveContainer] {
		if (!this->show_content_) {
			return text("Select a file from the file browser window!") | center;
		}


		std::string textHeader;
		if (this->is_Decrypting_) {
			textHeader = "Select the same algorithm that was used to encrypt the file";
		} else {
			textHeader = "Select encryption algorithm:";
		}

		// Update the placeholder text
		std::string algoHeader = this->algosDict[this->selectedRadioBtn_]["prompt"];

		Elements elem = {
				text(is_Decrypting_ ? "---Decrypting---" : "---Encrypting---") | center,
				separatorEmpty(),
				hbox({
						// Display the path to the file clicked.
						text("Selected file: ") | bold,
						text((selected_file_path_.string() != "") ? shorten_path(selected_file_path_.string()) : "No file selected") | color(Color::CyanLight),
				}) | center,
				separatorEmpty(),
				text(textHeader),
				hbox({
						text("    "), // indent
						radioboxComp_->Render(),
				}),
				separatorEmpty(),
		};

		// Check if an algo has been selected to display the text input field.
		if (this->selectedRadioBtn_ != 0) {
			elem.push_back(text(algoHeader));
			elem.push_back(hbox({
					separatorEmpty(),
					separatorEmpty(),
					hashInput_Comp_->Render() | border | bgcolor(Color::White) | color(Color::White),
					separatorEmpty(),
					separatorEmpty(),
			}));
			if (!this->hashInput_.empty()) { // if the user has entered text, then prompt to create a new file.
				elem.push_back(text("Create a new file:"));
				elem.push_back(hbox({
						separatorEmpty(),
						separatorEmpty(),
						newFileNameInput_Comp_->Render() | border | bgcolor(Color::White) | color(Color::White),
						separatorEmpty(),
						separatorEmpty(),
				}));
				if (!this->newFileNameInput_.empty()) {
					elem.push_back(vbox({
							separatorEmpty(),
							paragraph(std::format("Select a folder from the File browser window to save the {} file:", this->is_Decrypting_ ? "Decrypted" : "Encrypted")),
							separatorEmpty(),
					}));
					if (!this->selected_folder_to_save_path_.string().empty()) {
						elem.push_back(vbox({
								hbox({
										separatorEmpty() | flex,
										text(std::format("Selected folder to save to: {}", shorten_path(this->selected_folder_to_save_path_.string()))),
										separatorEmpty() | flex,
								}),
								hbox({
										separatorEmpty() | flex,
										actionButton_->Render(),
										separatorEmpty() | flex,
								}),
						}));
					}
				}
			}
		}

		return vbox(elem);
	});

	// Add() builds the component hierarchy
	Add(content);
}

// Class methods
void CryptoWindow::onSaveBtn()
{

	// TODO use the clear function to clear all crypto state variables.
	// this->reset();
}

std::vector<std::string> CryptoWindow::setup_algo_radio()
{
	std::vector<std::string> arr;

	// Grab each algos from the `name` key
	for (int i = 0; i < this->algosDict.size(); i++) {
		const auto algo = this->algosDict.at(i);
		const std::string algoName = algo.at("name");
		arr.push_back(algoName);
	}
	return arr;
}
