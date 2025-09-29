#include <pthread/sched.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <unistd.h>

#include <cstring>
#include <memory> // for allocator
#include <string>
#include <vector>

// FTXUI
#include <ftxui/component/component.hpp>		  // User input, Components
#include <ftxui/component/screen_interactive.hpp> // User input
#include <ftxui/dom/elements.hpp>				  // for filler, text, hbox, vbox, vflex
#include <ftxui/dom/node.hpp>					  // for Render
#include <ftxui/screen/color.hpp>				  // for ftxui
#include <ftxui/screen/screen.hpp>				  // for Full, Screen

#include "ftxui/component/component_base.hpp"	 // for ComponentBase
#include "ftxui/component/component_options.hpp" // for InputOption
#include "ftxui/dom/elements.hpp"
#include "ftxui/util/ref.hpp" // for Ref

// Link my header files, notice the "" vs the <>
#include "algos/AES.h"
#include "ftxui_components/fs_nodes.h"
#include "ftxui_components/window_components.h"
#include "utils/get_ascii_art.h"
#include "utils/string_utils.h"

using namespace ftxui;
// using std::cout;
// namespace fs = std::filesystem; // name space for file-system

int main()
{
	Element ascii_art = get_ascii_art();
	std::string selected_file_path = "";

	// Callback function when user click file button
	auto on_file_selected_callback = [&](const std::string &path) {
		selected_file_path = path;

		// TODO logic to perform callback function
	};

	// --- Components Creation ---

	// Get the user's home directory as the starting point of the collapsible
	// list
	std::string home_dir = std::getenv("HOME") ? getenv("HOME") : ".";
	auto file_browser = Make<CreateDirectoryNode>(home_dir, on_file_selected_callback);

	// component to display the path to the file clicked.
	auto selected_path_display = Renderer([&] {
		return hbox({
				   text("Selected file: ") | bold,
				   text((selected_file_path != "") ? shorten_path(selected_file_path) : "No file selected") |
					   color(Color::CyanLight),
			   }) |
			   center;
	});

	// First window is shows user how to use the program
	Component window_1 = Window({
		.inner = CreateIntroWindow(),
		.title = "How to use",
		.width = 50,
		.height = 30,
		.top = 5,
	});

	// The second is where the user selects the file.
	Component window_2 = Window({
		.inner = file_browser | vscroll_indicator | frame,
		.title = "Select a file",
		.width = 80,
		.left = 30,
		.height = 40,
		.top = 2,
	});

	// The third window where the user encrypts or decrypts the selected file
	Component window_3 = Window({
		// .inner = DummyWindowContent(),
		.title = "Cryptography",
		.left = 45,
		.top = 0,
		.width = 50,
		.height = 55,
	});

	auto all_windows_container = Container::Stacked({
		window_1,
		window_2,
		window_3,
	});

	// In FTXUI u can't just drop a Component's render result into another
	// renderer, you must make the component itself part of the hierarchy.
	// Otherwise you lose the event handling, so no interactive events are
	// captured.
	auto main_layout = Renderer(all_windows_container, [all_windows_container, ascii_art, selected_path_display] {
		Element top_part = vbox({
							   ascii_art | center,
							   separator(),
							   //    text("Select a file to encrypt or decrypt:")
							   //    | center | bold | color(Color::Cyan1),
							   selected_path_display->Render(),
						   }) |
						   size(HEIGHT, EQUAL, 30) | flex_grow;

		return vbox({
				   top_part,
				   all_windows_container->Render(),
			   }) |
			   border;
	});

	// Run the loop with the main layout component.
	ScreenInteractive::Fullscreen().Loop(main_layout);

	return 0;
}