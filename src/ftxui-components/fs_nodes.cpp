// File System Nodes

#include "ftxui_components/fs_nodes.h"

Component CreateFileNode(fs::path path, std::function<void(std::string)> on_file_selected_callback)
{
	auto renderer = Renderer([name = path.filename().string()](bool focused) {
		auto element = text("📄 " + name);
		// When the component is focused, i.e., user hovers over it with
		// cursor, display it with inverted colors.ƒ
		if (focused)
		{
			element |= inverted;
			// Tell the parent frame to scroll to this element.
			element |= focus;
		}
		return element;
	});

	// Make the component respond to the ENTER key.
	return CatchEvent(renderer, [on_file_selected_callback, path_str = path.string()](Event e) {
		if (e == Event::Return)
		//   for handling left mouse clicks
		//   if (e == Event::Return || (e.is_mouse() && e.mouse().button ==
		//   Mouse::Left && e.mouse().motion == Mouse::Pressed))
		{
			on_file_selected_callback(path_str);
			return true;
		}
		return false; // event not handled
	});
}

// Class Constructor implementation:
CreateDirectoryNode::CreateDirectoryNode(fs::path path, std::function<void(std::string)> on_file_selection_callback)
	: path_(std::move(path)), on_file_selected_callback_(std::move(on_file_selection_callback))
{
	// Use a flexible renderer for the header, so we can dynamically change the
	// arrow of directory whether its open or not.
	auto header = Renderer([this](bool focused) {
		// Determine the arrow based on the open state (is_open_)
		std::string arrow = is_open_ ? "↓ " : "→ ";

		// Get the display name and handle cases where the filepath is a root
		// "/" or ends with a backslash "/Users/username/test/"
		std::string display_name = path_.filename().string();
		if (display_name.empty())
		{
			display_name = path_.string();
		}

		// Create the text element for the header.
		Element element = text(arrow + display_name);

		// Add a visual indicator if the directory is focused, i.e, user is
		// hovering over it.
		if (focused)
		{
			element |= inverted;
			// Tell the parent frame to scroll to this element
			element |= focus;
		}
		return element;
	});

	// make the header interactive. It should handle the `Enter` key bring
	// pressed and left mouse click.
	auto interactive_header = CatchEvent(header, [this](Event e) {
		if (e == Event::Return)
		//  For handling left mouse clicks
		//  if (e == Event::Return || (e.is_mouse() && e.mouse().button ==
		//  Mouse::Left && e.mouse().motion == Mouse::Pressed))
		{
			is_open_ = !is_open_;
			if (is_open_ && !children_loaded_)
			{
				// if the directory is opened and the children elements
				// haven't been loaded
				LoadContents();
			}
			return true;
		}
		return false; // Event not handled
	});

	// The children are initially an empty placeholder.
	// They are only shown if `is_open` is true.
	auto indented_children =
		Renderer(children_placeholder_, [this] { return hbox({text("  "), children_placeholder_->Render()}); });

	// Create children components that will only render if`is_open_` is true
	auto conditional_children = indented_children | Maybe(&is_open_);

	// Add the button and the placeholder to this component
	Add(Container::Vertical({interactive_header, conditional_children}));
}

// Class Member function Implementation
void CreateDirectoryNode::LoadContents()
{ // Only runs when needed, i.e., user
	// clicks to open a
	// directory

	children_loaded_ = true;
	// Store all the children components in a list
	std::vector<Component> children;
	std::vector<fs::directory_entry> entries;

	try
	{
		for (const auto &entry : fs::directory_iterator(path_))
		{
			entries.push_back(entry);
		}
	}
	catch (const fs::filesystem_error &)
	{
		// cout << "ERROR LOADING FILE"; // TODO finish errors, handle
		// permission errors by adding a disabled text element
		children_placeholder_->Add(Renderer([] { return text("🚫 Access Denied") | dim; }));
		return;
	}

	// Sort the entries. Directories first aphabetically.
	std::sort(entries.begin(), entries.end(), [](const auto &a, const auto &b) {
		bool is_a_dir = a.is_directory();
		bool is_b_dir = b.is_directory();

		if (is_a_dir != is_b_dir)
		{
			return is_a_dir; // directories come first
		}
		return a.path().filename() < b.path().filename(); // then sort by name
	});

	for (const auto &entry : entries)
	{
		if (entry.is_directory())
		{
			// Recursively create another CreateDirectoryNode for
			// subdirectories
			children.push_back(Make<CreateDirectoryNode>(entry.path(), on_file_selected_callback_));
		}
		else if (entry.is_regular_file())
		{
			// Files are interactive components
			children.push_back(CreateFileNode(entry.path(), on_file_selected_callback_));
		}
	}

	// Replace the placeholder with a new container holding real children.
	children_placeholder_->DetachAllChildren(); // clear the placeholder
	children_placeholder_->Add(Container::Vertical(std::move(children)));
}
