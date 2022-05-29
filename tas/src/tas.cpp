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
		std::string file_string = IO::read_file(input_filename);
		try
		{
			std::vector<Token> file_tokens = tokenize::tokenize(file_string);
			for (Token const& token : file_tokens)
			{
				std::cout << token << '\n';
			}
		}
		catch (std::runtime_error const& e)
		{
			std::cerr << "File: " << input_filename << '\n' << e.what();
			return 1;
		}
	}
	return 0;
}