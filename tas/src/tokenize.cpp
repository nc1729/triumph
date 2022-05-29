#include <vector>
#include <string>
#include <iostream>

#include "error.h"
#include "tokenize.h"

std::vector<std::string> tokenize::remove_comments(std::vector<std::string> const& file_lines)
{
	bool in_block_comment = false;
	bool in_string = false;

	// loop over lines in file
	std::vector<std::string> new_lines;
	size_t line_number = 1;
	for (std::string const& line : file_lines)
	{
		std::string new_line = "";
		size_t index = 0;
		size_t line_length = line.length();
		while (index < line_length)
		{
			char this_char = line[index];
			if (in_block_comment)
			{
				// check if we've hit the end of the block comment
				if (this_char == '*' && index + 1 != line_length)
				{
					char next_char = line[index + 1];
					if (next_char == '/')
					{
						// found a block comment terminator "*/"
						in_block_comment = false;
						index++; // jump over next_char '/'
					}
				}
			}
			else if (in_string)
			{
				if (this_char == '\\' && index + 1 != line_length)
				{
					// might have found an escaped " char
					char next_char = line[index + 1];
					if (next_char == '\"')
					{
						// " char was escaped, so stay in the string
						// need to keep the \ char, because we'll process strings later
						new_line += '\\\"';
						index++;
					}
				}
				else if (this_char == '\"')
				{
					// found an unescaped " char
					in_string = false;
					new_line += '\"';
				}
				else
				{
					// still in string, add char to it, even if it's a comment char
					new_line += this_char;
				}
			}
			else
			{
				if (this_char == '/' && index + 1 != line_length)
				{
					// might have entered a comment
					char next_char = line[index + 1];
					if (next_char == '/')
					{
						// found a line comment, so break out of line loop
						break;
					}
					else if (next_char == '*')
					{
						// found a block comment "/*"
						in_block_comment = true;
						index++;
					}
					else
					{
						new_line += this_char;
					}
				}
				else if (this_char == '\"')
				{
					// found the start of a string
					in_string = true;
					new_line += '\"';
				}
				else
				{
					new_line += this_char;
				}
			}
			index++;
		}
		new_lines.push_back(new_line);
		line_number++;
	}

	// if in block comment or string at end of file, throw an error
	if (in_string)
	{
		// strings must be terminated at the end of the file
		error(new_lines[line_number - 1], "Unexpected end of file - string is not terminated.", line_number - 1);
	}
	if (in_block_comment)
	{
		// block comments must be terminated at the end of the file
		error(new_lines[line_number - 1], "Unexpected end of file - block comment is not terminated.", line_number - 1);
	}

	return new_lines;
	
}

std::vector<Token> tokenize::tokenize(std::vector<std::string> file_contents)
{
	file_contents = remove_comments(file_contents);
	std::vector<Token> plums;
	return plums;
}