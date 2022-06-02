#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>

#include "CLOptions.h"
#include "error.h"
#include "constants.h"
#include "tokenize.h"
#include "parse.h"
#include "assemble.h"
#include "link.h"
#include "Token.h"
#include "Block.h"
#include "FileIO.h"
#include "util.hpp"
#include "tas.h"

int tas(CLOptions const& options)
{
	std::vector<Block> code_blocks;

	// assemble all blocks in files in turn
	for (auto const& input_filename : options.input_filenames)
	{
		std::string file_string = IO::read_file(input_filename);
		std::vector<std::string> file_lines = util::break_file_into_lines(file_string);
		try
		{
			// break file into tokens
			std::vector<Token> file_tokens = tokenize::tokenize(file_string);
			// parse these tokens into code blocks, containing statements
			std::vector<Block> file_blocks = parse::parse(file_tokens, input_filename);
			// assemble statements in blocks, converting them into assembled ternary
			// except for jump destinations, which are handled when all blocks are collected together
			std::vector<Block> assembled_blocks = assemble::assemble(file_blocks);
			// append this file's blocks onto the global list of blocks
			code_blocks.insert(code_blocks.end(), assembled_blocks.begin(), assembled_blocks.end());
		}
		catch (TASError const& e)
		{
			std::cerr << "File: " << input_filename << '\n';
			std::cerr << "Line: " << e.line_number << "\n\n";
			std::cerr << "\"" << file_lines[e.line_number - 1] << "\"" << "\n\n";
			std::cerr << "Error: " << e.what() << "\n\n";
			return 1;
		}
	}

	// fetch start address for main from command line, if given
	Tryte const main_address = fetch_address(options.start_addr);

	// now link the code blocks together
	std::string output_string;
	try
	{
		std::vector<std::string> block_names = link::check_block_names(code_blocks);
		std::map<std::string, Tryte> block_addresses = link::arrange_blocks(code_blocks, main_address);
		code_blocks = link::link_blocks(code_blocks, block_addresses);
		output_string = link::output_assembly(code_blocks);
	}
	catch (TASLinkError const& e)
	{
		std::cerr << "File: " << e.filename << '\n';
		std::cerr << "Block name: " << e.block_name << '\n';
		std::cerr << "Error: " << e.what() << "\n\n";
		return 1;
	}
	catch (std::runtime_error const& e)
	{
		std::cerr << "Link error: " << e.what();
		return 1;
	}
	
	// finally, print ternary assembly to a file (and do padding if required)
	IO::write_file(options.output_filename, output_string);
	std::cout << output_string << '\n';

	return 0;
}

Tryte const fetch_address(std::string const& tryte_string)
{
	try
	{
		return Tryte(tryte_string);
	}
	catch(const std::exception& e)
	{
		(void)e;
		return constants::start_address;
	}
	
}