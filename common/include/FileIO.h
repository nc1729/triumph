#pragma once

#include <vector>
#include <string>
#include <fstream>

namespace IO
{
	std::vector<std::string> read_lines(std::string const& input_filename);
	std::string read_file(std::string const& input_filename);
	void write_file(std::string const& output_filename, std::string const& file_string);
}