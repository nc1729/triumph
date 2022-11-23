#include <vector>
#include <string>
#include <fstream>
#include <sstream>

#include "common/Tryte.h"
#include "common/FileIO.h"

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

void IO::write_file(std::string const& output_filename, std::string const& file_string)
{
    std::ofstream output_file(output_filename);
    if (output_file)
    {
        output_file << file_string;
    }
}

std::vector<Tryte> IO::read_trytes_from_file(std::string const& filename)
{
    std::vector<Tryte> program;
    std::string schars = "MLKJIHGFEDCBA0abcdefghijklm";
    char c;
    std::string tryte_str;
    std::ifstream input_file{filename};
    while (input_file >> c)
    {
        // ignore any non-septavingt chars
        if (schars.find(c) != std::string::npos)
        {
            // c is a valid septavingt char
            tryte_str += c;
            if (tryte_str.size() == 3)
            {
                // found a whole Tryte, add it to program
                program.emplace_back(tryte_str);
                tryte_str.clear();
            }
        }
    }

    return program;
}