// File System Nodes
#include <algorithm>
#include <filesystem>
#include <functional>
#include "AppModel.h"
#include "ftxui/dom/elements.hpp"

#include "components/fs_nodes.h"

Component CreateFileNode(AppModel &model_, fs::path path, std::function<void(fs::path)> on_file_selected_callback)
{
	ButtonOption headerOption;
	headerOption.transform = [name = path.filename().string()](const EntryState &s) {
		auto element = text("📄 " + name);
		// When the component is focused, i.e., user hovers over it with
		// cursor, display it with inverted colors.ƒ
		if (s.focused) {
			element |= inverted;
			// Tell the parent frame to scroll to this element.
			element |= focus;
		}
		return element;
	};
	return Button("", [on_file_selected_callback, path] { on_file_selected_callback(path); }, headerOption);
}


// Class Constructor implementation:
CreateDirectoryNode::CreateDirectoryNode(
	AppModel &model,
	fs::path path,
	std::function<void(fs::path)> on_file_selection_callback,
	std::function<void()> on_directory_selection_callback)

	// Set this Class's respective member variables to what is being passed in.
	:
	model_(model),
	path_(std::move(path)),
	on_file_selected_callback_(std::move(on_file_selection_callback)),
	on_directory_selection_callback_(on_directory_selection_callback)
{
	ButtonOption headerOption;
	headerOption.transform = [&](const EntryState &s) {
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
		if (s.focused) {
			element |= inverted;
			// Tell the parent frame to scroll to this element
			element |= focus;
		}
		return element;
	};

	// NOTE: A header in this case is one line of the filesystem in the File browser window.
	//  Example:
	//          ↓ Downloads  // is a header
	//              ↓ file.text // is a header, located inside of ./Downloads
	//          → Documents // is a header

	// This button allows the user to use both mouse left clicks and arrow keys to open a directory.
	auto header_button = Button("", [this] {
		// Check what the users goal is: to open and view a directory or to select that directory as the directory to save a file too.
		if (this->model_.isSelectingSaveDirectory) {
			this->model_.selected_folder_to_save_to_path = path_.string();
			this->on_directory_selection_callback_(); //make the crypto window take focus, i.e, become front window
			this->model_.isSelectingSaveDirectory = false;
			return; // consume event
		}

		is_open_ = !is_open_;
		if (is_open_ && !children_loaded_) {
			// If the directory is opened and the children elements haven't been loaded.
			LoadContents();
		}
	},
								headerOption);


	// The children are initially an empty placeholder. They are only shown if `is_open` is true.
	auto indented_children = Renderer(children_placeholder_, [this] { return hbox({text("  "), children_placeholder_->Render()}); });

	// Create children components that will only render if`is_open_` is true
	auto conditional_children = indented_children | Maybe(&is_open_);

	auto component_container = Container::Vertical({header_button, conditional_children});

	// Wrap the component is a Renderer lambda to check for the event of make an opened directories refresh to show newly added files.
	auto refresh_wrapper = Renderer(component_container, [this, component_container] {
		if (is_open_ && !this->model_.directoryToRefresh.empty() && model_.directoryToRefresh == path_) {
			LoadContents(); // Reload its contents again from disk
			model_.directoryToRefresh = fs::path{};
		}
		return component_container->Render();
	});

	Add(refresh_wrapper);
}

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
			children.push_back(Make<CreateDirectoryNode>(this->model_, entry.path(), on_file_selected_callback_, on_directory_selection_callback_));
		} else if (entry.is_regular_file()) {
			// Files are interactive components
			children.push_back(CreateFileNode(this->model_, entry.path(), on_file_selected_callback_));
		}
	}
	// Replace the placeholder with a new container holding real children.
	children_placeholder_->DetachAllChildren(); // clear the placeholder
	children_placeholder_->Add(Container::Vertical(std::move(children)));
}
