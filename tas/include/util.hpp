#pragma once

#include <vector>
#include <algorithm>
#include <string>

/*
handy utilities for parsing strings etc
*/
namespace util
{
	/*
	wrapper around std::find
	*/
	template<typename T>
	bool is_in(std::vector<T> const& vec, T const& v)
	{
		return std::find(vec.begin(), vec.end(), v) != vec.end();
	}

	std::vector<std::string> break_file_into_lines(std::string const& file_string);

	// convert string to upper case
	std::string to_upper(std::string const& word);
	// convert int to Tryte string
	std::string int_to_tryte_str(int64_t const& n);

	// return true if a string is an int
	bool string_is_int(std::string const& word);
	// return true if string is tryte (three chars in [a-m0A-M])
	bool string_is_tryte(std::string const& word);
	// return true if string is a septavingt const (e.g. 0sMMM)
	bool string_is_septavingt(std::string const& word);
	// return true if string is a ternary const (e.g. 0t+++0+--0)
	bool string_is_ternary(std::string const& word);
	// return true if string is a char const (e.g. 'x')
	bool string_is_char(std::string const& word);
	// return true if string begins with $ and is followed by 
	bool string_is_addr(std::string const& word);
	// return true if string begins with !
	bool string_is_jump_label(std::string const& word);
}