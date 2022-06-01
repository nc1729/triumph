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
		: std::runtime_error(err_msg.c_str()), line_number{ line_number } {};
};

// TASLinkError
// inherits from std::runtime_error
class TASLinkError : public std::runtime_error
{
public:
	std::string filename;
	std::string block_name;
	TASLinkError(std::string const& err_msg, std::string const& filename, std::string const& block_name)
		: std::runtime_error(err_msg.c_str()), filename{ filename }, block_name{ block_name } {};
};