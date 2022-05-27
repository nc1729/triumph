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

	// return true if a string is an int
	bool string_is_int(std::string const& word);
	// return true if string is a septavingt const (e.g. 0sMMM)
	bool string_is_septavingt(std::string const& word);
	// return true if string is a ternary const (e.g. 0t+++0+--0)
	bool string_is_ternary(std::string const& word);

}