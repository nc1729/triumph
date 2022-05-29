#pragma once

#include <vector>
#include <string>

#include "Token.h"

namespace tokenize
{
	// remove all comments from file
	std::string remove_comments(std::string const& file_string);
	// remove strings and store them in a list, with the strings replaced with placeholders
	std::vector<std::string> store_strings(std::string& file_string);
	// work through the file and construct a list of Tokens
	std::vector<Token> make_tokens(std::string const& file_string,
		std::vector<std::string> const& stored_strings);
	std::vector<Token> tokenize(std::string& file_string);
}