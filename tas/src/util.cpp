#include <string>
#include <cctype> // for std::toupper
#include <stdexcept>
#include <algorithm>

#include "constants.h"
#include "util.hpp"

std::string util::to_upper(std::string const& word)
{
	std::string out;
	for (char c : word)
	{
		out += std::toupper(c);
	}
	return out;
}
bool util::string_is_int(std::string const& word)
{
	try
	{
		int x = std::stoi(word);
		x = 1; // just to lose 'x is unused' error!
	}
	catch (std::invalid_argument)
	{
		return false;
	}
	return true;
}

bool util::string_is_tryte(std::string const& word)
{
	return std::all_of(word.begin(), word.end(),
		[](char c) {return constants::septavingt_chars.find(c) != std::string::npos; });
}

bool util::string_is_septavingt(std::string const& word)
{
	if (word.length() != 5)
	{
		return false;
	}
	else if (word.substr(0, 2) != "0s")
	{
		return false;
	}
	std::string tryte = word.substr(2);
	return string_is_tryte(tryte);
}

bool util::string_is_ternary(std::string const& word)
{
	if (word.length() != 11)
	{
		return false;
	}
	else if (word.substr(0, 2) != "0t")
	{
		return false;
	}
	std::string tryte = word.substr(2);
	return std::all_of(tryte.begin(), tryte.end(),
		[](char c) {return constants::ternary_chars.find(c) != std::string::npos; });
}

bool util::string_is_addr(std::string const& word)
{
	if (word[0] != '$') return false;
	std::string tryte = word.substr(1);
	return string_is_tryte(tryte);
}

bool util::string_is_jump_label(std::string const& word)
{
	return word[0] == '!';
}