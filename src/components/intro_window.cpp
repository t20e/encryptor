#include <ftxui/component/component.hpp> // User input, Radiobox, etc...

#include "components/intro_window.h"
#include "utils/ftxui_utils.h"

using namespace ftxui;

Component CreateIntroWindow()
{
	auto content = Renderer([] {
		const std::string intro = "This program allows you to encrypt and decrypt files. It is built on C++, utiling the GTest library for testing, and the FTXUI library to display in the terminal.";

		return vbox({
			separatorEmpty(), // Separates without adding a line between.
			paragraph(intro),
			separatorEmpty(),
			text("How to use:"),
			separatorEmpty(),
			IndentText("1. Click the `File browser` window to select the file you want to encrypt or decrypt."),
			separatorEmpty(),
			IndentText("2. After, selecting a file click the `Cryptography` window. You can scroll the `File previewer` window horizontally and vertically"),
			separatorEmpty(),
			IndentText("Note: You can drag and resize the windows! You can only decrypt files that were encrypted using this program.") |
				flex,
			text("`CTRL+c` to quit.") | bold,

		});
	});
	return content;
}
