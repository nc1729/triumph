#pragma once

#include <vector>
#include <string>

#include "Token.h"

namespace tokenize
{
	void remove_comments(std::vector<std::string> file_contents);
	std::vector<Token> tokenize(std::vector<std::string> file_contents);
}