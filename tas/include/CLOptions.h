#pragma once
#include <vector>
#include <string>

#include "constants.h"

/*
CLOptions:
Struct to hold information from command line interface.
*/
struct CLOptions
{
    std::vector<std::string> input_filenames;
    std::string output_filename;
    std::string start_addr;

    // default constructor
    CLOptions() : input_filenames{}, output_filename{ "a.tri" }, start_addr{constants::start_address} {};
};