#include <vector>
#include <string>
#include <iostream>

#include "tokenize.h"

std::vector<Token> tokenize::tokenize(std::vector<std::string> file_contents)
{
	for (auto const& line : file_contents)
	{
		std::cout << line << '\n';
	}
	std::vector<Token> plums;
	return plums;
}