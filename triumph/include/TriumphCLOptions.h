#pragma once
#include <vector>
#include <string>

#include "constants.h"

/*
TriumphCLOptions:
Struct to hold information from command line interface.
*/
struct TriumphCLOptions
{
    std::vector<std::string> disk_filenames;
    std::string input_trifile;
    std::string start_addr;
    bool debug_mode;
    bool console_mode;

    // default constructor
    TriumphCLOptions() : disk_filenames{}, input_trifile{}, start_addr{"M00"}, debug_mode{false}, console_mode{false} {};
};