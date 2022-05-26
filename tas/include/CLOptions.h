#pragma once
#include <vector>
#include <string>

/*
CLOptions:
Struct to hold information from command line interface.
*/
struct CLOptions
{
    bool Wall = false;
    bool Werror = false;
    bool debug = false;
    std::vector<std::string> input_filenames;
    std::string output_filename;

    // default constructor
    CLOptions() : Wall(false), Werror(false), debug(false),
        input_filenames{}, output_filename{ "a.tri" } {};
};