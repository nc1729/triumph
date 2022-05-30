#pragma once

#include <vector>
#include <string>
#include <map>

#include "Tryte.h"
#include "Statement.h"

struct Block
{
	std::string name;
	std::vector<Statement> statements;
	Tryte address;
	size_t length = 0;
	std::string assembly;
	std::map<std::string, Tryte> jump_offsets;

	Block(std::string const& name, std::vector<Statement> const& statements) :
		name{ name }, statements{ statements } {};
};

std::ostream& operator<<(std::ostream& os, Block const& block);