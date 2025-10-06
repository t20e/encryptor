
#include <format>
#include <string>

#include <ftxui/component/component.hpp> // User input, Radiobox
#include <ftxui/dom/elements.hpp> // for filler, text, hbox, vbox, vflex
#include "ftxui/component/component_options.hpp" // for InputOption
#include "ftxui/dom/elements.hpp"

// My headers
#include "AppModel.h"
#include "algos/caesar_cipher.h"
#include "components/collapsible.h"
#include "components/crypto_window.h"
#include "utils/file_utils.h"
#include "utils/ftxui_utils.h"
#include "utils/string_utils.h"

using namespace ftxui;

// Class constructor
CryptoWindow::CryptoWindow(
	AppModel &model,
	std::function<void()> resetApp) :
	model_(model),
	resetApp_(resetApp)
{
	// Setup the radio component with the items from algoDict
	std::vector<std::string> algo_radio_ = setup_algo_radio();

	// Radiobox Event that runs when the user selects an algo from the radiobox list.
	auto on_radiobox_algo_change = [this] {
		this->hashInput_ = ""; // Clear the text in the input field
		this->informUserHowToDecrypt = "";
	};

	// Initialize the component to empty.

	// Add input options:
	RadioboxOption radio_option;
	radio_option.on_change = on_radiobox_algo_change;

	InputOption newFolderInputOptions;
	InputOption hashInputOptions;

	// A Lambda event to track when user types into (newFileNameInput_).
	newFolderInputOptions.on_change = [this] {
		if (!model_.isDecrypting) {
			// --- Add `.enc` extension to the end of filename as user types.
			const std::string filenameEnding = ".enc";
			if (!newFileNameInput_.ends_with(filenameEnding)) {
				newFileNameInput_ += filenameEnding;
			}
		}
	};

	// A lambda to track when a user types into the (hashInput_).
	hashInputOptions.on_change = [this] {
		if (this->hashInput_.empty()) {
			return;
		}
		performCryptography();
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

	// Main lambda Renderer function re-runs everytime the UI needs to be redrawn, for each frame. ALL DYNAMIC LOGIC GOES INSIDE IT.
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
		std::string algoInstruction = this->model_.algosDict[this->selectedRadioBtn_]["prompt"];

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
			elem.push_back(text(algoInstruction));

			if (!this->err_msg.empty()) {
				elem.push_back(IndentText(this->err_msg, Color::Red));
			}

			elem.push_back(
				hbox({
					separatorEmpty(),
					separatorEmpty(),
					hashInput_Comp_->Render() | border | bgcolor(Color::White) | color(Color::White),
					separatorEmpty(),
					separatorEmpty(),

				}));

			if (!this->informUserHowToDecrypt.empty()) {
				elem.push_back(IndentText(this->informUserHowToDecrypt, Color::Cyan));
			}


			// if the user has entered text, then prompt to create a new file.
			if (!this->hashInput_.empty() && this->err_msg.empty()) {
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
					if (!this->model_.selected_folder_to_save_to_path.string().empty()) {
						elem.push_back(vbox({
							hbox({
								separatorEmpty() | flex,
								text(std::format("Selected folder to save to: {}", shorten_path(this->model_.selected_folder_to_save_to_path.string()))),
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
	saveFile(model_.outFileContents, newFileNameInput_, model_.selected_folder_to_save_to_path, model_.isDecrypting ? "" : model_.FILE_HEADER_IDENTIFIER);

	// Signal to refresh directory
	model_.directoryToRefresh = model_.selected_folder_to_save_to_path;

	resetCrypto();
	resetApp_();
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


void CryptoWindow::performCryptography()
{
	bool errorOccurred = false;

	switch (this->selectedRadioBtn_) {
		case 0: // None
			break;
		case 1: { // Caesar cipher
			cryptoResult result = caesarCipher(this->model_.selectedFileContents, this->hashInput_);
			// USe lambda std::visit to safely process the result
			std::visit([this, &errorOccurred](auto &&arg) {
				// Determine the type
				using T = std::decay_t<decltype(arg)>;

				if constexpr (std::is_same_v<T, std::vector<unsigned char>>) {
					// Success
					this->model_.outFileContents = arg;
					if (!this->model_.isDecrypting) {
						this->informUserHowToDecrypt = this->model_.algosDict[1]["informUserHowToDecrypt"] + " -" + this->hashInput_;
					}
				} else if constexpr (std::is_same_v<T, std::string>) {
					// Error
					this->err_msg = arg;
					errorOccurred = true;
				}
			},
					   result);
			break;
		}
		default:
			break;
	}
	if (errorOccurred) {
		this->model_.outFileContents.clear();
		this->informUserHowToDecrypt = "";
	} else {
		this->err_msg = "";
	}
}
