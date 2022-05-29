#pragma once

#include <string>

// error handling for tas
// takes the line of the file the error was found on, an error message, and the line number
void error(std::string const& line, std::string const& err_msg, size_t line_number);