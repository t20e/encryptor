// FTXUI Window components

#pragma once

#include <ftxui/component/component.hpp> // User input

/**
 * @brief Create the first window which shows the user how to user the program.
 * @return The first window component
 */
ftxui::Component CreateIntroWindow();

/**
 * @brief Create the Cryptography window.
 * @return The crytography window component.
 */
ftxui::Component CreateCryptoWindow();