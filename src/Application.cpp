#include <memory>

// FTXUI
#include "ftxui_components/intro_window.h"
#include <ftxui/component/component.hpp>		  // User input, Components
#include <ftxui/component/screen_interactive.hpp> // User input
#include <ftxui/dom/elements.hpp>				  // for filler, text, hbox, vbox, vflex
#include <ftxui/dom/node.hpp>					  // for Render
#include <ftxui/screen/color.hpp>				  // for ftxui
#include <ftxui/screen/screen.hpp>				  // for Full, Screen

#include "ftxui_components/fs_nodes.h"
#include "ftxui_components/intro_window.h"

// My headers
#include "Application.h"
#include "ftxui_components/crypto_window.h"
#include "utils/get_ascii_art.h"

Application::Application()
{ // Constructor

	// --- Create the FTXUI components
	cryptoWindow = Make<CryptoWindow>();

	fileBrowser = Make<CreateDirectoryNode>(home_dir, [this](const std::string &path) { 
        this->onFileSelected(path); 
        // cryptoWindow->
    });

	// Setup the 3 windows
	introWindow = Window({
		.inner = CreateIntroWindow(),
		.title = "How to use",
		.width = 50,
		.height = 30,
		.top = 5,
	});

	// This is where the user selects the file.
	fileBrowserWindow = Window({
		.inner = fileBrowser | vscroll_indicator | frame,
		.title = "File browser",
		.width = 80,
		.left = 20,
		.height = 40,
		.top = 2,
	});

	// This window where the user encrypts or decrypts the selected file
	cryptoWindow_container = Window({
		.inner = cryptoWindow,
		.title = "Cryptography",
		.left = 35,
		.top = 0,
		.width = 80,
		.height = 55,
	});

	all_windows_container = Container::Stacked({
		introWindow,
		fileBrowserWindow,
		cryptoWindow_container,
	});

	// --- Lambda functions that run per frame ----
	main_layout = Renderer(all_windows_container, [this] {
		Element top_part = vbox({
							   get_ascii_art() | center,
							   separatorEmpty(),
						   }) |
						   size(HEIGHT, EQUAL, 25) | flex_grow;

		return vbox({
				   top_part,
				   all_windows_container->Render(),
			   }) |
			   border;
	});
}

// --- CALLBACK METHOD IMPLEMENTATIONS ---

// Callback function when user clicks a file.
void Application::onFileSelected(const std::string &path)
{
	cryptoWindow->setSelectedFilePath(path);

	// Show contains of the cryptography window and make that window on top
	cryptoWindow->setShowContent(true);

	// TODO Check if the file has the header from above, to check if its an encrypted file, then uncomment below
	// line window->setIsDecrypting(true);

	// Bring the cryptography window to the front
	cryptoWindow_container->TakeFocus();
}

void Application::Run()
{
	// Run the loop with the main layout component.
	ScreenInteractive::Fullscreen().Loop(main_layout);
}