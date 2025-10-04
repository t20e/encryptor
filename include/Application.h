#pragma once

#include <ftxui/component/component.hpp> // User input, Components
#include <ftxui/component/screen_interactive.hpp> // User input


#include "AppModel.h"
#include "components/crypto_window.h"
#include "components/file_preview_window.h"


using namespace ftxui;


/**
 * @brief A main application to maintain structure, all other classes are called
 * from here.
 *
 */
class Application
{
	public:
		Application(); // Constructor
		void Run(); // Method to start the FTXUI event loop

	private:
		// --- State Member Variables ---
		std::unique_ptr<AppModel> model_;

		// Get the user's home directory as the starting point of the collapsible list
		fs::path home_dir_ = std::getenv("HOME") ? fs::path(getenv("HOME")) : fs::current_path();


		// --- FTXUI Components ---
		// In FTXbUI u can't just drop a Component's render result into another renderer, you must make the component itself part of the hierarchy. Otherwise, you lose the event handling, so no interactive events are captured.
		std::shared_ptr<FilePreview> filePreview_;

		Component fileBrowser; // rendered inside the fileBrowserWindow

		std::shared_ptr<CryptoWindow> cryptoWindow_;

		// A container that holds all the windows.
		Component all_windows_container_;

		// A main layout component that encompasses all the other components.
		Component main_layout_;

		/**
		 * @brief Callback function when user selects a file from the file browser window.
		 *
		 * @param path Full path to that file.
		 */
		void onFileSelected(const std::filesystem::path &path);


		/**
         * @brief Reset to default after file has been saved.
         * 
         */
		void reset();
};
