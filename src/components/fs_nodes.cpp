// File System Nodes
#include <algorithm>
#include <filesystem>
#include <functional>
#include "AppModel.h"
#include "ftxui/dom/elements.hpp"

#include "components/fs_nodes.h"

Component CreateFileNode(AppModel &model_, fs::path path, std::function<void(fs::path)> on_file_selected_callback)
{
	auto renderer = Renderer([name = path.filename().string()](bool focused) {
		auto element = text("📄 " + name);
		// When the component is focused, i.e., user hovers over it with
		// cursor, display it with inverted colors.ƒ
		if (focused) {
			element |= inverted;
			// Tell the parent frame to scroll to this element.
			element |= focus;
		}
		return element;
	});

	// Event: When user clicks `ENTER` key on a file.
	return CatchEvent(renderer, [on_file_selected_callback, path = path.string()](Event e) {
		if (e == Event::Return)
		//   for handling left mouse clicks
		//   if (e == Event::Return || (e.is_mouse() && e.mouse().button ==
		//   Mouse::Left && e.mouse().motion == Mouse::Pressed))
		{
			on_file_selected_callback(path);
			return true;
		}
		return false; // event not handled
	});
}

// Class Constructor implementation:
CreateDirectoryNode::CreateDirectoryNode(
		AppModel &model,
		fs::path path,
		std::function<void(fs::path)> on_file_selection_callback)

	// Set this Class's respective member variables to what is being passed in.
	:
	model_(model),
	path_(std::move(path)),
	on_file_selected_callback_(std::move(on_file_selection_callback))
{
	// NOTE: A header in this case is one line of the filesystem in the File browser window.
	//  Example:
	//          ↓ Downloads  // is a header
	//              ↓ file.text // is a header, located inside of ./Downloads
	//          → Documents // is a header

	// Use a flexible renderer for the header, so we can dynamically change the arrow of directory whether its open or not.
	auto header = Renderer([this](bool focused) {
		// Determine the arrow based on the open state (is_open_)
		std::string arrow = is_open_ ? "↓ " : "→ ";

		// Get the display name and handle cases where the filepath is a root "/" or ends with a backslash "/Users/username/test/"
		std::string display_name = path_.filename().string();
		if (display_name.empty()) {
			display_name = path_.string();
		}

		// Create the text element for the header.
		Element element = text(arrow + display_name);

		// Add a visual indicator if the directory is focused, i.e, user is hovering over it.
		if (focused) {
			element |= inverted;
			// Tell the parent frame to scroll to this element
			element |= focus;
		}
		return element;
	});

	// Event: When user click `ENTER` key on directory, open it and show its contents
	auto interactive_header = CatchEvent(header, [this](Event e) {
		if (e == Event::Return)
		//  For handling left mouse clicks
		//  if (e == Event::Return || (e.is_mouse() && e.mouse().button ==
		//  Mouse::Left && e.mouse().motion == Mouse::Pressed))
		{
			is_open_ = !is_open_;
			if (is_open_ && !children_loaded_) {
				// if the directory is opened and the children elements haven't been loaded
				LoadContents();

				// Only send the selected file if a file to encrypt or decrypt has been selected and passed to the cryptography window.
				if (this->model_.selected_file_path.string() != "") {
					this->model_.selected_folder_to_save_to_path_ = path_.string();
				}
			}
			return true;
		}
		return false; // Event not handled
	});

	// The children are initially an empty placeholder. They are only shown if `is_open` is true.
	auto indented_children = Renderer(children_placeholder_, [this] { return hbox({text("  "), children_placeholder_->Render()}); });

	// Create children components that will only render if`is_open_` is true
	auto conditional_children = indented_children | Maybe(&is_open_);

	// Add the button and the placeholder to this component
	Add(Container::Vertical({interactive_header, conditional_children}));
}

// TODO add `use arrows keys to navigate above the Home directory`
// Class Member function Implementation
void CreateDirectoryNode::LoadContents()
{
	// Only runs when needed, i.e., user clicks to open a directory
	children_loaded_ = true;
	// Store all the children components in a list
	std::vector<Component> children;
	std::vector<fs::directory_entry> entries;

	try {
		for (const auto &entry: fs::directory_iterator(path_)) {
			entries.push_back(entry);
		}
	} catch (const fs::filesystem_error &) {
		// permission errors by adding a disabled text element
		children_placeholder_->Add(Renderer([] { return text("🚫 Access Denied") | dim; }));
		return;
	}

	// Sort the entries. Directories first alphabetically.
	std::sort(entries.begin(), entries.end(), [](const auto &a, const auto &b) {
		bool is_a_dir = a.is_directory();
		bool is_b_dir = b.is_directory();

		if (is_a_dir != is_b_dir) {
			return is_a_dir; // directories come first
		}
		return a.path().filename() < b.path().filename(); // then sort by name
	});

	for (const auto &entry: entries) {
		if (entry.is_directory()) {
			// Recursively create another CreateDirectoryNode for
			// subdirectories
			children.push_back(Make<CreateDirectoryNode>(
					this->model_,
					entry.path(),
					on_file_selected_callback_));
		} else if (entry.is_regular_file()) {
			// Files are interactive components
			children.push_back(CreateFileNode(this->model_, entry.path(), on_file_selected_callback_));
		}
	}
	// Replace the placeholder with a new container holding real children.
	children_placeholder_->DetachAllChildren(); // clear the placeholder
	children_placeholder_->Add(Container::Vertical(std::move(children)));
}
