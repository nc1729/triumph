#include <vector>
#include <string>
#include <fstream>
#include <sstream>

#include "FileIO.h"
#include "Tryte.h"

std::vector<std::string> IO::read_lines(std::string const& input_filename)
{
	std::ifstream file(input_filename);
    if (file)
    {
        std::vector<std::string> lines;
        std::string line;
        while (std::getline(file, line))
        {
            lines.push_back(line);
        }
        return lines;
    }
    else
    {
        throw std::runtime_error("File " + input_filename + " could not be read.");
    }
}

std::string IO::read_file(std::string const& input_filename)
{
    std::ifstream input_file(input_filename);

    if (input_file)
    {
        std::stringstream file_contents;
        file_contents << input_file.rdbuf();
        return file_contents.str();
    }
    else
    {
        throw std::runtime_error("File " + input_filename + " could not be read.");
    }
}