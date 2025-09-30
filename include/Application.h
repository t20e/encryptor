#pragma once
#include "ftxui_components/crypto_window.h"
#include <ftxui/component/component.hpp> // User input, Components
#include <ftxui_components/intro_window.h>

using namespace ftxui;

/**
 * @brief A main application to maintain structure, all other classes are called from here.
 *
 */
class Application
{
  public:
	Application(); // Constructor
	void Run();	   // Method to start the FTXUI event loop

  private:
	// --- State Variables ---
	bool is_Decrypting = false;

	// Get the user's home directory as the starting point of the collapsible list
	std::string home_dir = std::getenv("HOME") ? getenv("HOME") : ".";

	// Define a header to add the to beginning of an encrypted file, to identify it.
	// Header -> ENC01_1_
	//      the     _(int)_ is the algo identifier,
	//                _1_ -> caesar_cipher, _2_ -> XOR, _3_ -> AES, etc...
	const std::string FILE_HEADER = "ENC01";

	// --- FTXUI Components ---
	// In FTXUI u can't just drop a Component's render result into another renderer, you must make the component itself part of the hierarchy. Otherwise you lose the event handling, so no interactive events are captured.

	Component introWindow;
	// The second is where the user selects the file.
	Component fileBrowserWindow;
	Component fileBrowser; // rendered inside the fileBrowserWindow

	std::shared_ptr<CryptoWindow> cryptoWindow;
	Component cryptoWindow_container;

	// The third window where the user encrypts or decrypts the selected file
	Component window_3;

	// A container that holds all the windows.
	Component all_windows_container;

	// A main layout component that encompasses all the other components.
	Component main_layout;

	// Callback Logic member functions, to propage state variables from the other classes.
	/**
	 * @brief Callback function when user selects a file from the file browser window.
	 *
	 * @param path Full path to that file.
	 */
	void onFileSelected(const std::string &path);
};