#include <string>
#include <cctype> // for std::toupper
#include <stdexcept>
#include <algorithm>

#include "constants.h"
#include "util.hpp"

std::vector<std::string> util::break_file_into_lines(std::string const& file_string)
{
	std::vector<std::string> lines;
	std::string line;
	for (char c : file_string)
	{
		if (c == '\n')
		{
			lines.push_back(line);
			line = "";
		}
		else
		{
		line += c;
		}
	}
	return lines;
}

bool util::is_uppercase(std::string const& word)
{
	return std::all_of(word.begin(), word.end(), [](char c) {return c >= 'A' && c <= 'Z'; });
}

bool util::is_lowercase(std::string const& word)
{
	return std::all_of(word.begin(), word.end(), [](char c) {return c >= 'a' && c <= 'z'; });
}

std::string util::to_upper(std::string const& word)
{
	std::string out;
	for (char c : word)
	{
		out += static_cast<char>(std::toupper(c));
	}
	return out;
}

std::string util::to_lower(std::string const& word)
{
	std::string out;
	for (char c : word)
	{
		out += static_cast<char>(std::tolower(c));
	}
	return out;
}

bool util::string_is_int(std::string const& word)
{
	return (word.find_first_not_of("0123456789") == std::string::npos);
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

bool util::string_is_char(std::string const& word)
{
	if (word.length() != 3)
	{
		return false;
	}
	else if (word[0] != '\'' || word[2] != '\'')
	{
	return false;
	}
	else
	{
		return true;
	}
}

bool util::string_is_control_char(std::string const& word)
{
	if (word.length() != 4)
	{
		return false;
	}
	else if (word[0] != '\'' || word[3] != '\'')
	{
		return false;
	}
	else
	{
		if (word[2] == 'a' || word[2] == 't' || word[2] == 'r' || word[2] == 'n')
		{
			return true;
		}
		return false;
	}
}
bool util::string_is_addr(std::string const& word)
{
	if (word[0] != '$') return false;
	std::string tryte = word.substr(1);
	return string_is_tryte(tryte);
}

bool util::string_is_septavingt_chars(std::string const& word)
{
	return word.find_first_not_of(constants::septavingt_chars) == std::string::npos;
}

bool util::string_is_jump_label(std::string const& word)
{
	return word[0] == '!';
}
