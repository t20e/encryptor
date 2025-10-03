#include <algorithm>
#include <cstddef>
#include <filesystem>
#include <memory>

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

	// --- Create the FTXUI components
	cryptoWindow_ = Make<CryptoWindow>();
	FilePreview_ = Make<FilePreview>();

	fileBrowser = Make<CreateDirectoryNode>(
			home_dir_,
			[this](const fs::path &filepPath) { this->onFileSelected(filepPath); },
			[this](const fs::path &folderPath) { this->cryptoWindow_->setSelectedFolderPath(folderPath); },
			[this] { return this->cryptoWindow_->getSelectedFilePath(); });

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
			FilePreview_,
	});

	// --- Lambda functions that run per frame ----
	main_layout_ = Renderer(main_layout_, [this] {
		Element top_element;
		if (true) {
			top_element = FilePreview_->Render();
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

// --- CALLBACK METHOD IMPLEMENTATIONS ---

// Callback function when user clicks a file.
void Application::onFileSelected(const fs::path &path)
{
	cryptoWindow_->setSelectedFilePath(path);

	// Show contains of the cryptography window and make that window on top
	cryptoWindow_->setShowContent(true);

	// TODO Check if the file has the header from above, to check if its an
	// encrypted file, then uncomment below line window->setIsDecrypting(true);


	// Bring the cryptography window to the front
	cryptoWindow_->TakeFocus();
}

void Application::Run()
{
	// Run the loop with the main layout component.
	ScreenInteractive::Fullscreen().Loop(main_layout_);
}
