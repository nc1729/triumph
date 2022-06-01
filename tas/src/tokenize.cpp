#include <vector>
#include <string>
#include <iostream>
#include <sstream>

#include "error.h"
#include "tokenize.h"

std::string tokenize::remove_comments(std::string const& file_string)
{
	bool in_block_comment = false;
	bool in_line_comment = false;
	bool in_string = false;

	// loop over characters in file
	std::string new_file_string;
	std::string new_line; // for error printouts
	size_t line_number = 1;
	size_t index = 0;
	while (index < file_string.length())
	{
		char this_char = file_string[index];
		if (in_block_comment)
		{
			// check if we've hit the end of the block comment
			if (this_char == '*' && index + 1 != file_string.length())
			{
				char next_char = file_string[index + 1];
				if (next_char == '/')
				{
					// found a block comment terminator "*/"
					in_block_comment = false;
					index++; // jump over next_char '/'
				}
			}
			else if (this_char == '\n')
			{
				// preserve newline chars
				new_line += this_char;
				new_file_string += new_line;
				new_line = "";
				line_number += 1;
			}
		}
		else if (in_line_comment)
		{
			if (this_char == '\n')
			{
				// found the end of the line - line comment // finishes
				in_line_comment = false;
				// preserve newline chars
				new_line += this_char;
				new_file_string += new_line;
				new_line = "";
				line_number += 1;
			}
		}
		else if (in_string)
		{
			// check if we've hit an escaped string char \"
			if (this_char == '\\' && index + 1 != file_string.length())
			{
				char next_char = file_string[index + 1];
				if (next_char == '\"')
				{
					// " char was escaped, so stay in the string
					// need to keep the \ char, because we'll process strings later
					new_line += "\\\"";
					index++;
				}
			}
			else if (this_char == '\"')
			{
				// found an unescaped " char
				in_string = false;
				new_line += '\"';
			}
			else if (this_char == '\n')
			{
				// newline found in the middle of a string - throw an error
				// to have long strings need to start a new string on the next like
				// "just like"
				// "this."
				throw TASError("Unexpected end of line - string is not terminated", line_number);
			}
			else
			{
				new_line += this_char;
			}
		}
		else
		{
			if (this_char == '/' && index + 1 != file_string.length())
			{
				// might have entered a comment
				char next_char = file_string[index + 1];
				if (next_char == '/')
				{
					// found a line comment
					in_line_comment = true;
					index++;
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
			else if (this_char == '\n')
			{
				// preserve newline chars
				new_line += this_char;
				new_file_string += new_line;
				new_line = "";
				line_number += 1;
			}
			else
			{
				new_line += this_char;
			}
		}
		index++;
	}
	// collect the last line!
	new_file_string += new_line;

	// reached the end of the file
	// block comments must be terminated
	if (in_block_comment)
	{
		throw TASError("Unexpected end of file - block comment is not terminated.", line_number);
	}

	return new_file_string;
}

std::vector<std::string> tokenize::store_strings(std::string& file_string)
{
	bool in_string = false;

	// loop over characters in file
	std::vector<std::string> stored_strings;
	std::string new_file_string;
	std::string new_string;
	size_t index = 0;
	while (index < file_string.length())
	{
		char this_char = file_string[index];
		if (in_string)
		{
			if (this_char == '\\')
			{
				// there's always a next char as strings don't span multiple lines
				char next_char = file_string[index + 1];
				if (next_char == '\"')
				{
					// " char was escaped
					// drop the \ from the stored string
					new_string += '\"';
					index++;
				}
				else
				{
					new_string += '\\';
				}
			}
			else if (this_char == '\"')
			{
				// found the end of the string
				stored_strings.push_back(new_string);
				in_string = false;
			}
			else
			{
				new_string += this_char;
			}
		}
		else
		{
			if (this_char == '\"')
			{
				// found a string
				in_string = true;
				new_string = "";
				// add a space-separated placeholder to the original lines
				new_file_string += (" __STR__" + std::to_string(stored_strings.size()) + " ");
			}
			else if (this_char == ',')
			{
				// replace commas with whitespace
				new_file_string += ' ';
			}
			else
			{
				new_file_string += this_char;
			}
		}
		index++;
	}

	file_string = new_file_string;
	return stored_strings;
}

std::vector<Token> tokenize::make_tokens(std::string const& file_string, 
	std::vector<std::string> const& stored_strings)
{
	bool in_token = false;
	std::vector<Token> tokens;

	// loop through file, making tokens
	size_t line_number = 1;
	size_t index = 0;
	std::string token_word;
	while (index < file_string.length())
	{
		char this_char = file_string[index];
		if (in_token)
		{
			if (this_char == '\n')
			{
				// found the end of a token - and a line
				if (token_word.substr(0, 7) == "__STR__")
				{
					// found a string placeholder
					size_t string_number = std::stoi(token_word.substr(7));
					tokens.emplace_back(stored_strings[string_number], line_number, TokenType::STRING);
					// skip over the number at the end of the placeholder
					// but remember we increment index each loop
					index += token_word.substr(7).length() - 1;
					token_word = "";
					in_token = false;
				}
				else
				{
					tokens.emplace_back(token_word, line_number);
					token_word = "";
					in_token = false;
				}
				// add a newline token and increment line number
				token_word += this_char;
				tokens.emplace_back(token_word, line_number);
				token_word = "";
				line_number += 1;
			}
			else if (this_char == '\t' || this_char == ' ' || this_char == ',')
			{
				// found whitespace (or a comma), signifying the end of a token
				if (token_word.substr(0, 7) == "__STR__")
				{
					// found a string placeholder
					size_t string_number = std::stoi(token_word.substr(7));
					tokens.emplace_back(stored_strings[string_number], line_number, TokenType::STRING);
					// skip over the number at the end of the placeholder
					// but remember we increment index each loop
					index += token_word.substr(7).length() - 1;
					token_word = "";
					in_token = false;
				}
				else
				{
					tokens.emplace_back(token_word, line_number);
					token_word = "";
					in_token = false;
				}
			}
			else if (this_char == ';' || this_char == '{' || this_char == ',')
			{
				// these chars form their own tokens
				// finish the preceeding token
				if (token_word.substr(0, 7) == "__STR__")
				{
					// found a string placeholder
					size_t string_number = std::stoi(token_word.substr(7));
					tokens.emplace_back(stored_strings[string_number], line_number, TokenType::STRING);
					// skip over the number at the end of the placeholder
					// but remember we increment index each loop
					index += token_word.substr(7).length() - 1;
					token_word = "";
					in_token = false;
				}
				else
				{
					tokens.emplace_back(token_word, line_number);
					token_word = "";
					in_token = false;
				}
				// then add this special char to its own token
				token_word += this_char;
				tokens.emplace_back(token_word, line_number);
				token_word = "";
			}
			else
			{
				token_word += this_char;
			}
		}
		else
		{
			if (this_char == '\n')
			{
				// add a newline token and increment line number
				token_word += this_char;
				tokens.emplace_back(token_word, line_number);
				token_word = "";
				line_number += 1;
			}
			else if (this_char == '\t' || this_char == ' ' || this_char == ',')
			{
				// ignore whitespace and commas
			}
			else if (this_char == ';' || this_char == '{' || this_char == '}')
			{
				// semicolons and braces are special, they get their own token
				token_word += this_char;
				tokens.emplace_back(token_word, line_number);
				token_word = "";
			}
			else
			{
				// we've found the start of a token
				in_token = true;
				token_word += this_char;
			}
		}
		index++;
	}

	return tokens;
}

std::vector<Token> tokenize::tokenize(std::string& file_string)
{
	file_string = remove_comments(file_string);
	std::vector<std::string> stored_strings = store_strings(file_string);
	std::vector<Token> tokens = make_tokens(file_string, stored_strings);
	return tokens;
}