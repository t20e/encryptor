// FTXUI Window components

#include "ftxui/component/component.hpp"

// My headers
#include "ftxui_components/window_components.h"

using namespace ftxui;

Component CreateIntroWindow()
{
	auto content = Renderer([] {
		const std::string intro =
			R"(	This program allows you to encrypt and decrypt files. It is built on C++, utiling the FTXUI library for displaying over the terminal, and the GTest library for testing.)";

		auto indented_paragraph = [](const std::string &text_content) {
			return hbox({
				text("    "), // indentation
				paragraph(text_content),
			});
		};

		return vbox({
			separatorEmpty(), // Separates without adding a line between.
			paragraph(intro),
			separatorEmpty(),
			separatorEmpty(),
			indented_paragraph("1. Click the `Select a file` window to select the file you want to encrypt or decrypt. "
							   "[use arrow keys to navigate]"),
			separatorEmpty(),
			indented_paragraph("2. After, selecting a file click the `Cryptography` window."),
			separatorEmpty(),
			indented_paragraph("Note: You can drag and resize the windows! You can only decrypt files that were "
							   "encrypted using this program.") |
				flex,
			text("`CTRL+c` to quit.") | bold,

		});
	});
	return content;
}

Component CreateCryptoWindow()
{
    
}
