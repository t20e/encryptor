#include <ftxui/component/component.hpp> // User input, Radiobox, components
#include <ftxui/component/component_base.hpp>
#include <ftxui/dom/elements.hpp>

#include "components/file_preview_window.h"

using namespace ftxui;

std::string generate_long_text(const std::string &prefix)
{
	std::string result;
	for (int i = 1; i <= 150; ++i) {
		result += prefix + " line " + std::to_string(i) + "\n";
	}
	return result;
}

FilePreview::FilePreview()
{

	in_content_ = generate_long_text("Original");
	out_content_ = generate_long_text("Encrypted");

	auto content = Renderer([this] {
		Element left = paragraph(in_content_);
		Element right = paragraph(out_content_);

		return hbox({
				left | center | flex,
				separatorDashed(),
				right | center | flex,
		});
	});

	auto scrollable_content = Renderer(content, [&, content] {
		return content->Render() | focusPositionRelative(scroll_x_, scroll_y_) | frame | flex;
	});

	// SliderOption<float> option_x;
	// option_x.value = &scroll_x_;
	// option_x.min = 0.f;
	// option_x.max = 1.f;
	// option_x.increment = 0.1f;
	// option_x.direction = Direction::Right;
	// option_x.color_active = Color::Blue;
	// option_x.color_inactive = Color::BlueLight;
	// auto scrollbar_x = Slider(option_x);

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
							scrollable_content,
							scrollbar_y,
					}) | flex,
					// Container::Horizontal({
					// 		scrollbar_x,
					// 		Renderer([] { return text(L" "); }),
					// }),
			}) |
			border);
}
