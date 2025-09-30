#include "ftxui/dom/elements.hpp"

/**
 * @brief Indent an FTXUI element. Like text()
 *
 * @param text_content
 * @return ftxui::Element An hbox() element.
 */
inline ftxui::Element IndentText(const std::string &text_content)
{ // The `inline` allows the whole definition to be in this header file. Its an exception to ODR.
	return ftxui::hbox({
		ftxui::text("    "), // indentation
		ftxui::paragraph(text_content),
	});
}

// auto indented_paragraph = [](const std::string &text_content) {
// 	return hbox({
// 		text("    "), // indentation
// 		paragraph(text_content),
// 	});
// };