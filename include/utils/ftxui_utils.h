#pragma once

#include "ftxui/dom/elements.hpp"

using namespace ftxui;


/**
 * @brief Indent an FTXUI element. Like text()
 *
 * @param text_content Contents to indent.
 * @param Co The color to display the content in.
 * @return ftxui::Element An hbox() element.
 */
inline Element IndentText(const std::string &text_content, const Color &Co = Color::Black)
{ // The `inline` allows the whole definition to be in this header file. Its an exception to ODR.
	return hbox({
			text("    "), // indentation
			paragraph(text_content) | color(Co),
	});
}
