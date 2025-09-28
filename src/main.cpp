#include <memory>     // for allocator
#include <filesystem> // to read system files and folders
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>

#include <cstdio>
#include <cstring>
#include <fstream>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <unistd.h>

// FTXUI
#include "ftxui/component/captured_mouse.hpp"     // for ftxui
#include "ftxui/component/component_base.hpp"     // for ComponentBase
#include "ftxui/component/component_options.hpp"  // for InputOption
#include "ftxui/util/ref.hpp"                     // for Ref
#include <ftxui/component/component.hpp>          // User input
#include <ftxui/component/screen_interactive.hpp> // User input
#include <ftxui/dom/elements.hpp>                 // for filler, text, hbox, vbox, vflex
#include <ftxui/dom/node.hpp>                     // for Render
#include <ftxui/screen/color.hpp>                 // for ftxui
#include <ftxui/screen/screen.hpp>                // for Full, Screen

// Link my header files, notice the ""
#include "algos/AES.h"
#include "utils/get_ascii_art.h"
#include "utils/string_utils.h"

using namespace ftxui;
namespace fs = std::filesystem; // name space for file-system
using std::cout;

// Take a list of component, display them vertically, one column shifted to the
// right.
Component Inner(std::vector<Component> children)
{
    Component vlist = Container::Vertical(std::move(children));
    return Renderer(vlist, [vlist]
                    { return hbox({
                          text(" "),
                          vlist->Render(),
                      }); });
}

// Create empty component
Component Empty()
{
    return std::make_shared<ComponentBase>();
}

Component DummyWindowContent()
{
    class Impl : public ComponentBase
    {
    private:
        float scroll_x = 0.1;
        float scroll_y = 0.1;

    public:
        Impl()
        {
            auto content = Renderer([=]
                                    {
        const std::string lorem =
            "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed "
            "do eiusmod tempor incididunt ut labore et dolore magna "
            "aliqua. Ut enim ad minim veniam, quis nostrud exercitation "
            "ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis "
            "aute irure dolor in reprehenderit in voluptate velit esse "
            "cillum dolore eu fugiat nulla pariatur. Excepteur sint "
            "occaecat cupidatat non proident, sunt in culpa qui officia "
            "deserunt mollit anim id est laborum.";
        return vbox({
            text(lorem.substr(0, -1)),   text(lorem.substr(5, -1)),   text(""),
            text(lorem.substr(10, -1)),  text(lorem.substr(15, -1)),  text(""),
            text(lorem.substr(20, -1)),  text(lorem.substr(25, -1)),  text(""),
            text(lorem.substr(30, -1)),  text(lorem.substr(35, -1)),  text(""),
            text(lorem.substr(40, -1)),  text(lorem.substr(45, -1)),  text(""),
            text(lorem.substr(50, -1)),  text(lorem.substr(55, -1)),  text(""),
            text(lorem.substr(60, -1)),  text(lorem.substr(65, -1)),  text(""),
            text(lorem.substr(70, -1)),  text(lorem.substr(75, -1)),  text(""),
            text(lorem.substr(80, -1)),  text(lorem.substr(85, -1)),  text(""),
            text(lorem.substr(90, -1)),  text(lorem.substr(95, -1)),  text(""),
            text(lorem.substr(100, -1)), text(lorem.substr(105, -1)), text(""),
            text(lorem.substr(110, -1)), text(lorem.substr(115, -1)), text(""),
            text(lorem.substr(120, -1)), text(lorem.substr(125, -1)), text(""),
            text(lorem.substr(130, -1)), text(lorem.substr(135, -1)), text(""),
            text(lorem.substr(140, -1)),
        }); });

            auto scrollable_content = Renderer(
                content, [&, content]
                { return content->Render() | focusPositionRelative(scroll_x, scroll_y) |
                         frame | flex; });

            SliderOption<float> option_x;
            option_x.value = &scroll_x;
            option_x.min = 0.f;
            option_x.max = 1.f;
            option_x.increment = 0.1f;
            option_x.direction = Direction::Right;
            option_x.color_active = Color::Blue;
            option_x.color_inactive = Color::BlueLight;
            auto scrollbar_x = Slider(option_x);

            SliderOption<float> option_y;
            option_y.value = &scroll_y;
            option_y.min = 0.f;
            option_y.max = 1.f;
            option_y.increment = 0.1f;
            option_y.direction = Direction::Down;
            option_y.color_active = Color::Yellow;
            option_y.color_inactive = Color::YellowLight;
            auto scrollbar_y = Slider(option_y);

            Add(Container::Vertical({
                Container::Horizontal({
                    scrollable_content,
                    scrollbar_y,
                }) | flex,
                Container::Horizontal({
                    scrollbar_x,
                    Renderer([]
                             { return text(L"x"); }),
                }),
            }));
        }
    };
    return Make<Impl>();
}

/**
 * @brief Create a file/directory component that highlights when the user is hovering over it.
 *
 * @param path Path to the file or directory.
 * @param on_file_selected_callback Callback function when user selects a file.
 * @return Component
 */
Component CreateInteractiveFile(
    fs::path path,
    std::function<void(std::string)> on_file_selected_callback)
{

    auto renderer = Renderer([name = path.filename().string()](bool focused)
                             {
            auto element = text("📄 " + name);
            // When the component is focused, i.e., user selects it, display it with inverted colors
            if(focused){
                element = element | inverted;
            }
            return element; });

    // Make the component respond to the ENTER key.
    // TODO handle left mouse click
    return CatchEvent(renderer, [on_file_selected_callback, path_str = path.string()](Event e)
                      {
                          if (e == Event::Return)
                          {
                              on_file_selected_callback(path_str);
                              return true;
                          }
                          return false; // event not handled
                      });
}

// constructor takes the path and the callback for file selection.
class CreateFileTreeComponent : public ComponentBase
{
public:
    // Constructor
    CreateFileTreeComponent(fs::path path, std::function<void(std::string)> on_file_selection_callback) : path_(std::move(path)),
                                                                                                          on_file_selected_callback_(std::move(on_file_selection_callback))
    {

        // Use a flexible renderer for the header, so we can dynamically change the arrow of directory whether its open or not.
        auto header = Renderer([this](bool focused)
                               {
            // Determine the arrow based on the open state (is_open_)
            std::string arrow = is_open_ ? "↓ " : "→ ";

            // Get the display name and handle cases where the filepath is a root "/" or ends with a backslash "/Users/username/test/"
            std::string display_name = path_.filename().string();
            if(display_name.empty()){
                display_name = path_.string();
            }

            // Create the text element for the header.
            Element element = text(arrow + display_name);

            // Add a visual indicator if the directory is focused
            if(focused){
                element=element | inverted;
            }
            return element; });

        // make the header interactive. It should handle the `Enter` key bring pressed.
        auto interactive_header = CatchEvent(header, [this](Event event)
                                             {
                                                 if (event == Event::Return)
                                                 {
                                                     is_open_ = !is_open_;
                                                     if (is_open_ && !children_loaded_)
                                                     {
                                                         // if the directory is opened and the children elements haven't been loaded
                                                         LoadContents();
                                                     }
                                                     return true;
                                                 }
                                                 return false; // Event not handled
                                             });

        // The children are initially an empty placeholder.
        // They are only shown if `is_open` is true.
        auto indented_children = Renderer(children_placeholder_, [this]
                                          { return hbox({text("  "), children_placeholder_->Render()}); });

        // Create children components that will only render if`is_open_` is true
        auto conditional_children = indented_children | Maybe(&is_open_);

        // Add the button and the placeholder to this component
        Add(Container::Vertical({interactive_header,
                                 conditional_children}));
    }

private:
    void LoadContents()
    { // Only runs when needed, i.e., user clicks to open a directory

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
            // cout << "ERROR LOADING FILE"; // TODO finish errors, handle permission errors by adding a disabled text element
            children_placeholder_->Add(
                Renderer([]
                         { return text("🚫 Access Denied") | dim; }));
            return;
        }

        // Sort the entries. Directories first aphabetically.
        std::sort(entries.begin(), entries.end(), [](const auto &a, const auto &b)
                  {
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
                // Recursively create another CreateFileTreeComponent for subdirectories
                children.push_back(Make<CreateFileTreeComponent>(entry.path(), on_file_selected_callback_));
            }
            else if (entry.is_regular_file())
            {
                // Files are interactive buttons
                children.push_back(
                    // Button("📄 " + entry.path().filename().string(), [this, path = entry.path()]
                    //                       { on_file_selected_callback_(path.string()); }, ButtonOption::Ascii())
                    CreateInteractiveFile(entry.path(), on_file_selected_callback_));
            }
        }

        // Replace the placeholder with a new container holding real children.
        children_placeholder_->DetachAllChildren(); // clear the placeholder
        children_placeholder_->Add(Container::Vertical(std::move(children)));
    }

    // Private variables
    fs::path path_;
    std::function<void(std::string)> on_file_selected_callback_;
    bool is_open_ = false;
    bool children_loaded_ = false;
    Component children_placeholder_ = Container::Vertical({});
};

int main()
{
    Element ascii_art = get_ascii_art();
    std::string selected_file_path = "No file selected";

    // Callback function when user click file button
    auto on_file_selected_callback = [&](const std::string &path)
    {
        selected_file_path = path;
        // TODO logic to perform callback function
    };

    // --- Component Creation ---

    // Get the user's home directory as the starting point of the collapsible list
    std::string home_dir = std::getenv("HOME") ? getenv("HOME") : ".";
    auto file_browser = Make<CreateFileTreeComponent>(home_dir, on_file_selected_callback);

    // auto file_browser = CreateFileTreeComponent(home_dir, on_file_selected_callback);

    // component to display the path to the file clicked.
    auto selected_path_display = Renderer([&]
                                          { return hbox({
                                                       text("Selected: ") | bold,
                                                       text(selected_file_path) | color(Color::Cyan1),
                                                   }) |
                                                   center; });

    // The window_1 is where the user selects the file.
    // auto window_1_content = Renderer(file_browser, [file_browser]{ return file_browser->Render() | vscroll_indicator | frame ; });

    auto window_1 = Window({.inner = file_browser | vscroll_indicator | frame,
                            // .inner = window_1_content,
                            // .inner = DummyWindowContent(),
                            .title = "Select a file",
                            .width = 80,
                            .height = 40,
                            .top = 2});

    // The second window where the user encrypts or decrypts the selected file
    auto window_2 = Window({
        .inner = DummyWindowContent(),
        .title = "Cryptography",
        .left = 40,
        .top = 0,
        .width = 50,
        .height = 55,
    });

    auto all_windows_container = Container::Stacked({
        window_1,
        window_2,
    });

    // In FTXUI u cant just drop a Component's render result into another renderer, you must make the component itself part of the hierarchy. Otherwise you lose the event handling, so no interactive events are captured.
    auto main_layout = Renderer(all_windows_container, [all_windows_container, ascii_art, selected_path_display]
                                {
        Element top_part = vbox({
                               ascii_art | center,
                               separator(),
                            //    text("Select a file to encrypt or decrypt:") | center | bold | color(Color::Cyan1),
                            selected_path_display->Render(),
                           }) |   size(HEIGHT, EQUAL, 30) | flex_grow;

        return vbox({
                   top_part,
                   all_windows_container->Render(),
               }) |
               border; });

    // Run the loop with the main layout component.
    ScreenInteractive::Fullscreen().Loop(main_layout);
    // ScreenInteractive::Fullscreen().Loop(file_browser | vscroll_indicator | frame);

    return 0;
}