#include <iostream>

#include "CLOptions.h"
#include "tokenize.h"
#include "Tryte.h"
#include "FileIO.h"
#include "tas.h"

int64_t tas(CLOptions const& options)
{
	for (auto const& input_filename : options.input_filenames)
	{
		std::string file_contents = IO::read_file(input_filename);
		std::cout << file_contents << '\n';
		//tokenize::tokenize(lines);
	}
	return 0;
}