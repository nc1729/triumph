#include <iostream>

#include "Block.h"
#include "Statement.h"

std::vector<Statement>::iterator Block::begin()
{
	return statements.begin();
}

std::vector<Statement>::iterator Block::end()
{
	return statements.end();
}

std::vector<Statement>::const_iterator Block::begin() const
{
	return statements.cbegin();
}

std::vector<Statement>::const_iterator Block::end() const
{
	return statements.cend();
}

void Block::compute_tryte_length()
{
	length = 0;
	for (Statement& statement : statements)
	{
		if (statement.type != StatementType::JUMP_LABEL)
		{
			length += statement.tryte_length();
		}
	}
}

std::string Block::get_assembly() const
{
	std::string output;
	for (Statement const& statement : statements)
	{
		for (std::string const& tryte : statement.assembled_trytes)
		{
			output += tryte;
		}
	}
	return output;
}

std::ostream& operator<<(std::ostream& os, Block const& block)
{
	os << block.name << " {\n";
	for (Statement const& s : block)
	{
		os << s << '\n';
	}
	os << "}";
	return os;
}