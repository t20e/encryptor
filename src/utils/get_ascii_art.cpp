#include <iostream>
#include <fstream>

#include "utils/get_ascii_art.h"

#include <ftxui/dom/elements.hpp>

using std::cout;

// TODO is the colors needed since FTXUI is taking over?

// Read a ASCII art .txt file and return it as a FTXUI element.
ftxui::Element get_ascii_art()
{
    // Store all the lines of the text file
    std::vector<ftxui::Element> art_lines;

    // Open file stream
    std::ifstream file("misc/ascii.txt");
    std::string line;

    // Handle .txt file not found
    if (!file.is_open())
    {
        return ftxui::text("Error: ASCII art file not found!") | ftxui::center;
    }

    // Read file line by line, converting each line into a `text` element
    while (std::getline(file, line))
    {
        art_lines.push_back(ftxui::text(line));
    }

    // close file stream
    file.close();

    // Stack all the text elements vertically
    return ftxui::vbox(art_lines);
}