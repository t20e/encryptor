#pragma once

#include <filesystem> // to read system files and folders
#include <functional> // for std::function
#include <string>

// FTXUI
#include <ftxui/component/component.hpp> // User input
#include <ftxui/component/screen_interactive.hpp> // User input, CatchEvent
#include "ftxui/component/component_base.hpp" // for ComponentBase

namespace fs = std::filesystem; // name space for file-system
using namespace ftxui;

/**
 * @brief Create a directory node tree with child components.
 *
 * @param path Path to the file or directory.
 * @param on_file_selected_callback Callback function when user selects a file.
 * @return A FTXUI Component.
 */
Component CreateFileNode(fs::path path, std::function<void(fs::path)> on_file_selected_callback);

/**
 * @brief Create a directory node tree with child components.
 */
class CreateDirectoryNode : public ftxui::ComponentBase
{
	public:
		/**
		 * @brief Create a Directory Node object
		 *
		 * @param path Path to the file or directory.
		 * @param on_file_selection_callback Callback function when user selects a
		 * file.
		 * @param on_folder_select_callback Callback function when user selects a
		 * folder, for the purpose of saving a new file to it.
		 */
		CreateDirectoryNode(
				fs::path path,
				std::function<void(fs::path)> on_file_selection_callback,
				std::function<void(fs::path)> on_folder_select_callback,
				std::function<fs::path()> getSelectedFileFromCryptoWindow);

	private:
		void LoadContents();

		// Member variables
		fs::path path_;
		std::function<void(fs::path)> on_file_selected_callback_;
		std::function<void(fs::path)> on_folder_select_callback_;
		std::function<fs::path()> getSelectedFileFromCryptoWindow_;

		bool is_open_ = false;
		bool children_loaded_ = false;
		Component children_placeholder_ = Container::Vertical({});
};
