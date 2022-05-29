#include <iostream>
#include <vector>
#include <string>

#include "CLOptions.h"
#include "tokenize.h"
#include "Token.h"
#include "Tryte.h"
#include "FileIO.h"
#include "tas.h"

int tas(CLOptions const& options)
{
	for (auto const& input_filename : options.input_filenames)
	{
		std::vector<std::string> file_contents = IO::read_lines(input_filename);
		try
		{
			std::vector<Token> file_tokens = tokenize::tokenize(file_contents);
		}
		catch (std::runtime_error const& e)
		{
			std::cerr << "File: " << input_filename << '\n' << e.what();
			return 1;
		}
	}
	return 0;
}