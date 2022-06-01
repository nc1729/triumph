#pragma once

#include <vector>
#include <string>

#include "Token.h"
#include "Statement.h"
#include "Block.h"

namespace parse
{
	// take a list of Tokens generated from tokenizing a file
	// and make a set of Blocks by collecting Tokens into Statements
	// throw errors if syntax issues
	std::vector<Block> make_blocks(std::vector<Token> const& tokens, std::string const& filename);

	// if statement matches the form of a macro, expand it
	std::vector<Statement> expand_macro(Statement const& statement);

	// work through statements in all Blocks
	// if statement matches a macro, replace that statement with several statements
	std::vector<Block>& handle_macros(std::vector<Block>& blocks);

	// take a list of Tokens and collect them into code Blocks, containing a series
	// of statements. At this stage (before assembly) handle macros
	std::vector<Block> parse(std::vector<Token> const& tokens, std::string const& filename);
}
