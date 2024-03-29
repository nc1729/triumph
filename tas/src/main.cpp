#include <iostream>

#include "common/CLI11/CLI11.hpp"
#include "TASCLOptions.h"
#include "tas.h"


int main(int argc, char** argv) {
    CLI::App app{ "Converts a collection of ternary assembly (.tas) files \
     into a ternary machine code file (.tri)" };

    TASCLOptions options;

    // input files
    app.add_option("-i,--input_files,input", options.input_filenames, "One or more input files (relative to current directory)");

    // output filename
    app.add_option("-o,--output", options.output_filename, "Output filename: default is a.tri");

    // start address for main
    app.add_option("-a,--addr,--start_addr", options.start_addr, "main start address for linking: default is $DMM");

    // if verbose flag found, print assembly in readable form to stdout
    app.add_flag("-v", options.verbose, "Print assembly in readable form");

    // parse command line arguments, populating options
    CLI11_PARSE(app, argc, argv);

    // run assembler
    return tas(options);
}