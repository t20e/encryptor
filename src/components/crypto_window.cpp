
#include <format>
#include <string>

#include <ftxui/component/component.hpp> // User input, Radiobox
#include <ftxui/dom/elements.hpp> // for filler, text, hbox, vbox, vflex
#include "ftxui/component/component_options.hpp" // for InputOption
#include "ftxui/dom/elements.hpp"

// My headers
#include <utils/file_utils.h>
#include "AppModel.h"
#include "algos/caesar_cipher.h"
#include "components/crypto_window.h"
#include "utils/string_utils.h"

using namespace ftxui;

// Class constructor
CryptoWindow::CryptoWindow(AppModel &model) :
	model_(model)
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
	InputOption hashInputOptions;

	/**
     * @brief A Lambda event to track when user types into (newFileNameInput_).
     * 
     */
	newFolderInputOptions.on_change = [this] {
		// --- Add `.enc` extension to the end of filename as user types.
		const std::string filenameEnding = ".enc";
		if (!newFileNameInput_.ends_with(filenameEnding)) {
			newFileNameInput_ += filenameEnding;
		}
	};
    
	/**
     * @brief A lambda to track when a user types into the (hashInput_).
     * 
     */
	hashInputOptions.on_change = [this] {
		// TODO make it so that this lambda function doesn't run for every time the user types a character, maybe delay by a few seconds.
		if (this->hashInput_.empty()) {
			return;
		}

		std::vector<unsigned char> out;
		int algoInput = std::stoi(this->hashInput_);

		// --- Decrypt a file: ---
		if (this->model_.isDecrypting) {

		} else {
			// --- Encrypt a file: ---
			out = caesarCipher(this->model_.selectedFileContents, algoInput);
		}
		this->model_.outFileContents = out;
	};

	// Initialize the components
	radioboxComp_ = Radiobox(algo_radio_, &this->selectedRadioBtn_, radio_option);
	hashInput_Comp_ = Input(&this->hashInput_, "Input...", hashInputOptions);

	newFileNameInput_Comp_ = Input(&this->newFileNameInput_, "File name...", newFolderInputOptions);

	this->actionButton_ = Button("Save", [this] { this->onSaveBtn(); });

	// Group interactive components in a Container, which manages the events and focus between its children, then we can put them in an Element like vbox.
	auto interactiveContainer = Container::Vertical({
			radioboxComp_,
			hashInput_Comp_,
			newFileNameInput_Comp_,
			actionButton_,
	});

	// This lambda Renderer function re-runs everytime the UI needs to be redrawn, for each frame. ALL DYNAMIC LOGIC GOES INSIDE IT.
	auto content = Renderer(interactiveContainer, [this, interactiveContainer] {
		if (this->model_.selected_file_path.empty()) {
			return text("Select a file from the file browser window!") | center;
		}

		std::string textHeader;
		if (this->model_.isDecrypting) {
			textHeader = "Select the same algorithm that was used to encrypt the file";
		} else {
			textHeader = "Select encryption algorithm:";
		}

		// Update the placeholder text
		std::string algoHeader = this->model_.algosDict[this->selectedRadioBtn_]["prompt"];

		Elements elem = {
				text(this->model_.isDecrypting ? "---Decrypting---" : "---Encrypting---") | center,
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
							paragraph(std::format("Select a folder from the File browser window to save the {} file:", this->model_.isDecrypting ? "Decrypted" : "Encrypted")),
							separatorEmpty(),
					}));
					if (!this->model_.selected_folder_to_save_to_path_.string().empty()) {
						elem.push_back(vbox({
								hbox({
										separatorEmpty() | flex,
										text(std::format("Selected folder to save to: {}", shorten_path(this->model_.selected_folder_to_save_to_path_.string()))),
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
	saveFile(this->model_.selectedFileContents, this->newFileNameInput_, this->model_.selected_folder_to_save_to_path_);
	// TODO once save is implemented, call all reset() for all classes to go back to a clean state!


	// TODO The reset doesn't make the directory that has the new file saved to redrawn, so if I save a file to ./downloads and it was already opened then the file wont be shown in filebrowser. Maybe call filebrowser to draw it.
}

std::vector<std::string> CryptoWindow::setup_algo_radio()
{
	std::vector<std::string> arr;

	// Grab each algos from the `name` key
	for (int i = 0; i < this->model_.algosDict.size(); i++) {
		const auto algo = this->model_.algosDict.at(i);
		const std::string algoName = algo.at("name");
		arr.push_back(algoName);
	}
	return arr;
}
