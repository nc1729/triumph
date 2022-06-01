#pragma once

#include <vector>
#include <string>
#include <map>

#include "Tryte.h"
#include "Block.h"

namespace link
{
	// check that a 'main' block exists
	// and that two blocks haven't got the same name
	std::vector<std::string> check_block_names(std::vector<Block> const& blocks);
	// arrange the blocks in memory, starting from address of main (by default $0MM)
	// the main block is sent to the front of the blocks
	std::map<std::string, Tryte> arrange_blocks(std::vector<Block>& blocks);
	// using the address in memory of the blocks, resolve the placeholder names inside
	// each block in turn
	std::vector<Block>& link_blocks(std::vector<Block>& blocks,
		std::map<std::string, Tryte> const& block_addresses);
	// with everything linked and ready, output a final string of all the assembled ternary
	std::string output_assembly(std::vector<Block>& blocks);
}