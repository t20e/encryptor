#include <ftxui/component/component.hpp> // User input, Radiobox, etc...

#include "ftxui_components/intro_window.h"
#include "utils/ftxui_utils.h"

using namespace ftxui;

Component CreateIntroWindow()
{
	auto content = Renderer([] {
		const std::string intro = R"(	This program allows you to encrypt and decrypt files. It is built on C++, utiling the FTXUI library for displaying over the terminal, and the GTest library for testing.)";

		return vbox({
			separatorEmpty(), // Separates without adding a line between.
			paragraph(intro),
			separatorEmpty(),
			separatorEmpty(),
			IndentText("1. Click the `File browser` window to select the file you want to encrypt or decrypt."),
			separatorEmpty(),
			IndentText("2. After, selecting a file click the `Cryptography` window."),
			separatorEmpty(),
			IndentText("Note: You can drag and resize the windows! You can only decrypt files that were "
							"encrypted using this program.") |
				flex,
			text("`CTRL+c` to quit.") | bold,

		});
	});
	return content;
}
