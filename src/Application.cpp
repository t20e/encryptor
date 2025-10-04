
// FTXUI
#include <ftxui/component/component.hpp> // User input, Components
#include <ftxui/component/screen_interactive.hpp> // User input
#include <ftxui/dom/node.hpp> // for Render
#include <ftxui/screen/color.hpp> // for ftxui
#include <ftxui/screen/screen.hpp> // for Full, Screen
#include "ftxui/dom/elements.hpp"


// My headers
#include <utils/file_utils.h>
#include "Application.h"
#include "components/crypto_window.h"
#include "components/file_preview_window.h"
#include "components/fs_nodes.h"
#include "components/intro_window.h"
#include "utils/get_ascii_art.h"

namespace fs = std::filesystem; // name space for file-system
using namespace ftxui;

Application::Application()
{ // Constructor

	model_ = std::make_unique<AppModel>();

	// --- Create the FTXUI components
	filePreview_ = Make<FilePreview>(*model_);

	cryptoWindow_ = Make<CryptoWindow>(*model_);

	fileBrowser = Make<CreateDirectoryNode>(
			*model_,
			home_dir_,
			[this](const fs::path &filepPath) { this->onFileSelected(filepPath); });

	// -- Setup the windows
	all_windows_container_ = Container::Stacked({
			Window({
					.inner = CreateIntroWindow(),
					.title = "How to use",
					.width = 50,
					.height = 30,
					.top = 10,
			}),
			Window({
					.inner = fileBrowser | vscroll_indicator | frame,
					.title = "File browser",
					.width = 80,
					.left = 10,
					.height = 40,
					.top = 5,
			}),
			Window({
					.inner = cryptoWindow_,
					.title = "Cryptography",
					.left = 25,
					.top = 0,
					.width = 90,
					.height = 55,
			}),
	});

	main_layout_ = Container::Vertical({
			all_windows_container_,
			filePreview_,
	});

	// --- Lambda functions that run per frame ----
	main_layout_ = Renderer(main_layout_, [this] {
		Element top_element;
		if (model_->showFilePreviews) {
			top_element = filePreview_->Render();
		} else {
			top_element = get_ascii_art() | center;
		}

		// Make the top_element take 25% of the terminal size dynamically.
		auto terminalSize = Terminal::Size();
		int rows = terminalSize.dimy;
		int topRows = std::max(3, rows * 25 / 100);
		int maxTopRows = std::max(3, rows - 3);
		topRows = std::min(topRows, maxTopRows);

		return vbox({
				top_element | size(HEIGHT, EQUAL, topRows),
				all_windows_container_->Render() | flex,
		});
	});
}

// Callback function when user clicks a file.
void Application::onFileSelected(const fs::path &filePath)
{
	model_->selected_file_path = filePath;

	// Open the selected file
	model_->selectedFileContents = read_file_contents(filePath);

	// Clear the out file contents if it was previously filled.
	if (!model_->outFileContents.empty()) {
		model_->outFileContents.clear();
	}

	// Show the contents in file preview window.
	model_->showFilePreviews = true;

	// TODO Check if the file has the header from above, to check if its an
	// encrypted file, then uncomment below line window->setIsDecrypting(true);


	// Bring the cryptography window to the front
	cryptoWindow_->TakeFocus();
}

void Application::Run()
{ // Run the loop with the main layout component.
	ScreenInteractive::Fullscreen().Loop(main_layout_);
}

void Application::reset()
{
	// Clear the vector buffers
	model_->selectedFileContents.clear();
	model_->outFileContents.clear();

	model_->selectedFileContents.clear();
	model_->outFileContents.clear();

	model_->isDecrypting = false;

	model_->showFilePreviews = false;
	// Define a header to add the to beginning of an encrypted file, to identify it.
	//      Header -> ENCRYPTOR_1_
	//                 the   _(int)_ is the algo identifier,
	//                _1_ -> caesar_cipher, _2_ -> XOR, _3_ -> AES, etc...
	const std::string FILE_HEADER_ = "ENCRYPTOR";

	fs::path selected_file_path; // reset with fs::path{}
	fs::path selected_folder_to_save_to_path_;

}
