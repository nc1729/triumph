#include <iostream>

#include "CLOptions.h"
#include "tas.h"
#include "Tryte.h"

int64_t tas(CLOptions const& options)
{
	for (auto const& input_filename : options.input_filenames)
	{
		std::cout << input_filename << '\n';
	}
	return 0;
}