#pragma once

#include <vector>
#include <string>

#include "Token.h"

namespace tokenize
{
	std::vector<std::string> remove_comments(std::vector<std::string> const& file_lines);
	std::vector<Token> tokenize(std::vector<std::string> file_contents);
}