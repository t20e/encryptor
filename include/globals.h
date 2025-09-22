#ifndef GLOBALS_H
#define GLOBALS_H

// Only define variables in Header files if it is a CONST!!!

// ANSI escape codes
inline const char *CYAN = "\033[36m";
inline const char *UNDERLINED_CYAN = "\033[4;36m";
inline const char *GREEN = "\033[32m";
inline const char *RED = "\033[31m";
inline const char *PURPLE = "\033[35m";

inline const char *BOLD = "\033[1m";
inline const char *RESET = "\033[0m"; // reset all attributes

inline const char *MOVE_UP_ONE_LINE = "\033[1A"; // move the cursor up one line

#endif