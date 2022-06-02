#include <vector>
#include <string>
#include <map>
#include <stdexcept>
#include <algorithm>

#include "Tryte.h"
#include "Block.h"
#include "util.hpp"
#include "error.h"
#include "link.h"

std::vector<std::string> link::check_block_names(std::vector<Block> const& blocks)
{
	// add block names to vector, checking that there's no clashes
	std::vector<std::string> block_names;
	for (Block const& block : blocks)
	{
		if (util::is_in(block_names, block.name))
		{
			throw TASLinkError("Redefinition of code block", block.filename, block.name);
		}
		block_names.push_back(block.name);
	}

	// check that a block with name 'main' exists
	std::string main = "main";
	if (!util::is_in(block_names, main))
	{
		throw std::runtime_error("No main block found");
	}

	return block_names;
}

std::map<std::string, Tryte> link::arrange_blocks(std::vector<Block>& blocks)
{
	// find main block and swap it into the first position in blocks
	size_t index = 0;
	for (Block const& block : blocks)
	{
		if (block.name == "main")
		{
			break;
		}
		index++;
	}
	std::swap(blocks[index], blocks[0]);
	Block& main_block = blocks[0];

	// set main's starting address
	// this is fixed at $DMM for now, the first address after the bank memory
	main_block.address = Tryte("DMM");

	// collect lengths of blocks
	std::vector<size_t> block_lengths(blocks.size());
	for (size_t i = 0; i < blocks.size(); i++)
	{
		// block lengths were computed at assembly
		block_lengths[i] = blocks[i].length;
	}

	// check total size of output is less than maximum (13*729 Trytes)
	// maybe replace this with a warning?
	size_t total_size = 0;
	for (auto const& block_length : block_lengths)
	{
		total_size += block_length;
	}
	if (total_size > (13 * 729))
	{
		throw std::runtime_error("Total size of generated assembly greater than 9,477 Trytes");
	}

	// using the block lengths, arrange blocks in memory starting at main
	Tryte current = main_block.address + block_lengths[0];
	for (size_t i = 1; i < blocks.size(); i++)
	{
		blocks[i].address = current;
		current += block_lengths[i];
	}

	std::map<std::string, Tryte> block_addresses;
	for (Block const& block : blocks)
	{
		block_addresses[block.name] = block.address;
	}

	return block_addresses;
}

std::vector<Block>& link::link_blocks(std::vector<Block>& blocks,
	std::map<std::string, Tryte> const& block_addresses)
{
	// collect names of blocks
	std::vector<std::string> block_names;
	for (Block const& block : blocks)
	{
		block_names.push_back(block.name);
	}

	// loop over blocks and resolve jump labels and names
	for (Block& block : blocks)
	{
		for (Statement& statement : block.statements)
		{
			if (statement.type != StatementType::JUMP_LABEL) // ignore jump label statements
			{
				for (std::string& tryte : statement.assembled_trytes)
				{
					if (tryte[0] == '_')
					{
						// found a name
						std::string name = tryte.substr(1);
						if (block.jump_label_offsets.find(name) != block.jump_label_offsets.end())
						{
							// name is a local jump label
							// compute its Tryte address and replace the jump label
							tryte = Tryte::get_str(block.address + block.jump_label_offsets[name]);
						}
						else if (block_addresses.find(name) != block_addresses.end())
						{
							// name is a jump to another code block
							tryte = Tryte::get_str(block_addresses.at(name));
						}
						else
						{
							std::string err_msg = "Unresolved name " + name + " in JP statement";
							throw TASLinkError(err_msg, block.filename, block.name);
						}
					}
				}
			}
		}
	}
	return blocks;
}

std::string link::output_assembly(std::vector<Block>& blocks)
{
	std::string output;
	for (Block const& block : blocks)
	{
		output += block.get_assembly();
	}
	return output;
}