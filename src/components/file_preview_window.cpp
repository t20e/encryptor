#include <ftxui/component/component.hpp> // User input, Radiobox, components
#include <ftxui/component/component_base.hpp>
#include <ftxui/dom/elements.hpp>

#include "AppModel.h"
#include "components/file_preview_window.h"
#include "ftxui/dom/elements.hpp"
#include "utils/file_utils.h"
#include "utils/string_utils.h"

using namespace ftxui;

FilePreview::FilePreview(AppModel &model) :
	model_(model)
{
	auto scrollable_content = Renderer([this] {
		if (this->model_.selectedFileContents.empty()) {
			// Only render if there is content to display
			return hbox();
		}

		// Convert buffers to elements inside the renderer lambda so to update the contents when needed.
		Elements elems = {
				vbox({
						hbox({
								text("Selected File:") | color(Color::DeepPink1),
								text(shorten_path(this->model_.selected_file_path.string())),
						}),
						separatorLight(),
						convertBufferToElements(this->model_.selectedFileContents) |
								focusPositionRelative(scroll_x_, scroll_y_) | frame |
								flex,
				}) | flex,
		};

		if (!this->model_.outFileContents.empty()) {
			Element right_pane = vbox({
										 text(this->model_.isDecrypting ? "Decrypted File:" : "Encrypted File:") | color(Color::DeepPink1),
										 separatorLight(),
										 convertBufferToElements(this->model_.outFileContents) |
												 focusPositionRelative(scroll_x_, scroll_y_) | frame |
												 flex,
								 }) |
								 flex;
			elems.push_back(separatorEmpty());
			elems.push_back(separatorDashed());
			elems.push_back(separatorEmpty());
			elems.push_back(right_pane);
		}
		return hbox({elems});
	});

	// Create the scrollbar feature
	SliderOption<float> option_x;
	option_x.value = &scroll_x_;
	option_x.min = 0.f;
	option_x.max = 1.f;
	option_x.increment = 0.1f;
	option_x.direction = Direction::Right;
	option_x.color_active = Color::Blue;
	option_x.color_inactive = Color::BlueLight;
	auto scrollbar_x = Slider(option_x);

	SliderOption<float> option_y;
	option_y.value = &scroll_y_;
	option_y.min = 0.f;
	option_y.max = 1.f;
	option_y.increment = 0.1f;
	option_y.direction = Direction::Down;
	option_y.color_active = Color::Yellow;
	option_y.color_inactive = Color::YellowLight;
	auto scrollbar_y = Slider(option_y);


	Add(
			Container::Vertical({
					Container::Horizontal({
							scrollable_content | flex,
							scrollbar_y,
					}) | flex,
					Container::Horizontal({
							scrollbar_x,
							Renderer([] { return text(L" "); }),
					}),
			}) |
			border);
}

Element FilePreview::convertBufferToElements(const std::vector<unsigned char> &buffer)
{
	std::string display_string = std::string(buffer.begin(), buffer.end());

	// Split the string by newline characters and create a FTXUI text element for each line.
	Elements lines;
	std::stringstream ss(display_string);
	std::string line;

	while (std::getline(ss, line, '\n')) {
		lines.push_back(text(line));
	}

	if (lines.empty()) { // If the original string was empty.
		lines.push_back(text(""));
	}
	return vbox(lines);
}
