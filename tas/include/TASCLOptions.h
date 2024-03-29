#pragma once
#include <vector>
#include <string>

#include "common/constants.h"

/*
TASCLOptions:
Struct to hold information from command line interface.
*/
struct TASCLOptions
{
    std::vector<std::string> input_filenames;
    std::string output_filename;
    std::string start_addr;
    bool verbose;

    // default constructor
    TASCLOptions() : input_filenames{}, output_filename{ "a.tri" }, start_addr{constants::start_address}, 
    verbose{false} {};
};