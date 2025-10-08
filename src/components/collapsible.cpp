#include <ftxui/component/component.hpp>

#include "components/collapsible.h"
#include "utils/ftxui_utils.h"

using namespace ftxui;


Component createCollapsible(
	std::shared_ptr<std::string> algoName_ptr,
	std::shared_ptr<std::string> description_ptr)
{
	auto is_open = std::make_shared<bool>(false);

	ButtonOption btnOption;
	btnOption.transform = [algoName_ptr, is_open](const EntryState &s) {
		std::string arrow = *is_open ? "↓ " : "→ ";
		// Use (*) to dereference the point and get the current text value from that memory address
		Element elem = text(arrow + "About: " + *algoName_ptr);

		if (s.focused) {
			// Add a visual indicator if the directory is focused, i.e, user is hovering over it.
			elem |= inverted;
			elem |= focus;
		}
		return hbox({
			separatorEmpty() | flex_shrink,
			elem | flex,
			separatorEmpty() | flex_grow,
		});
	};

	// Button action
	auto headerBtn = Button("", [is_open] {
		*is_open = !(*is_open); // Flip the bool value at pointer memory address.
	},
							btnOption);
	auto content_renderer = Renderer([description_ptr] {
        return IndentText(*description_ptr);
	});

	// The Maybe decorator will only show the content if is_open = true.
	auto conditional_content = content_renderer | Maybe(is_open.get());

	return Container::Vertical({headerBtn, conditional_content});
}
