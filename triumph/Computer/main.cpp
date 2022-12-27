#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <thread>
#include <stdexcept>

#include "common/debug_utils.h"
#include "common/Tryte.h"
#include "common/CLI11/CLI11.hpp"
#include "Computer/TriumphCLOptions.h"
#include "Computer/Computer.h"
#include "IO/Disk.h"

// SDL messes with main, defining its own
// this undoes that, allowing the option to run without SDL in console mode
#undef main

int main(int argc, char** argv) {
    CLI::App app{ "Converts a collection of ternary assembly (.tas) files \
     into a ternary machine code file (.tri)" };

    TriumphCLOptions options;

    // input disk files
    app.add_option("-d,--disks", options.disk_filenames, "One or more input files (relative to current directory)");

    // input .tri file
    app.add_option("-i", options.input_trifile, ".tri file to run immediately (rather than from disk)");
    // start address for CPU
    app.add_option("-a,--addr,--start_addr", options.start_addr, "main start address for CPU: default is $DMM");

    // flag for console mode
    app.add_flag("-c,--console", options.console_mode, "TRIUMPH will run in console only.");

    // flag for debugger on startup
    app.add_flag("--debug", options.debug_mode, "TRIUMPH will start in debug mode.");

    // parse command line arguments, populating options
    CLI11_PARSE(app, argc, argv);

    std::vector<Disk> disks;
    for (std::string const& disk_name : options.disk_filenames)
    {
        // the first disk in the argument list will be assigned to bank 1, then bank 2 etc
        try
        {
            disks.emplace_back(disk_name);
        }
        catch (std::runtime_error const& e)
        {
            std::cerr << e.what() << '\n';
            return 1;
        }
    }

    try
    {
        Computer computer(disks, options);
        computer.turn_on();
    }
    catch(std::runtime_error const& e)
    {
        std::cerr << e.what() << '\n';
        return 1;
    }

    return 0;
}
