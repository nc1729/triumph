#pragma once

#include <string>
#include <stdexcept>

// TASError
// inherits from std::runtime_error, but passes the line number the error occurred on back up too
class TASError : public std::runtime_error
{
public:
	size_t line_number;
	TASError(std::string const& err_msg, size_t const line_number)
		: std::runtime_error(err_msg.c_str()), line_number{ line_number }
	{

	};
};