#include <ftxui/component/component.hpp>

#include "components/collapsible.h"
using namespace ftxui;


Component createCollapsible(
	std::string &algoName,
	std::string &description)
{
	auto is_open = std::make_shared<bool>(false);

	ButtonOption btnOption;
	btnOption.transform = [algoName, is_open](const EntryState &s) {
		std::string arrow = *is_open ? "↓ " : "→ ";
		Element elem = text(arrow + "About: " + algoName);

		if (s.focused) {
			// Add a visual indicator if the directory is focused, i.e, user is hovering over it.
			elem |= inverted;
			elem |= focus;
		}
		return elem;
	};

	// Button action
	auto headerBtn = Button("", [is_open] {
		*is_open = !(*is_open); // Flip the bool value at pointer memory address.
	},
							btnOption);
	auto content_renderer = Renderer([description] {
		return paragraph("    " + description);
	});

	// The Maybe decorator will only show the content if is_open = true.
	auto conditional_content = content_renderer | Maybe(is_open.get());

	return Container::Vertical({headerBtn, conditional_content});
}
