#include <iostream>

#include "CLI11/CLI11.hpp"
#include "CLOptions.h"
#include "tas.h"


int main(int argc, char** argv) {
    CLI::App app{ "Converts a collection of ternary assembly (.tas) files \
     into a ternary machine code file (.tri)" };

    // assembler flags
    CLOptions options;
    app.add_flag("--Wall", options.Wall, "Display all warnings");
    app.add_flag("--Werror", options.Werror, "Treat all warnings as errors");
    app.add_flag("--debug", options.debug, "Run in debug mode");

    // input files
    app.add_option("-i,--input_files,input", options.input_filenames, "One or more input files (relative to current directory)");

    // output filename
    app.add_option("-o,--output", options.output_filename, "Output filename: default is a.tri");

    // parse command line arguments, populating options
    CLI11_PARSE(app, argc, argv);

    // run assembler
    int64_t out_code = tas(options);
    return static_cast<int>(out_code);
}