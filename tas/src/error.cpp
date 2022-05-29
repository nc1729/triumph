#include <string>
#include <stdexcept>

#include "error.h"

void error(std::string const& line, std::string const& err_msg, size_t line_number)
{
	std::string err_where = "line " + std::to_string(line_number) + ": \n\n";

	throw std::runtime_error(err_where + line + "\n\n" + err_msg);
}