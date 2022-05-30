#include <iostream>
#include <vector>
#include <string>

#include "CLOptions.h"
#include "error.h"
#include "tokenize.h"
#include "parse.h"
#include "Token.h"
#include "Block.h"
#include "FileIO.h"
#include "util.hpp"
#include "tas.h"

int tas(CLOptions const& options)
{
	for (auto const& input_filename : options.input_filenames)
	{
		std::string file_string = IO::read_file(input_filename);
		std::vector<std::string> file_lines = util::break_file_into_lines(file_string);
		try
		{
			std::vector<Token> file_tokens = tokenize::tokenize(file_string);
			std::vector<Block> file_blocks = parse::parse(file_tokens);
			for (Block const& block : file_blocks)
			{
				std::cout << block << '\n';
			}
		}
		catch (TASError const& e)
		{
			std::cerr << "File: " << input_filename << '\n';
			std::cerr << "Line: " << e.line_number << "\n\n";
			std::cerr << "\"" << file_lines[e.line_number - 1] << "\"" << "\n\n";
			std::cerr << "Error: " << e.what() << "\n\n";
			return 1;
		}
	}
	return 0;
}