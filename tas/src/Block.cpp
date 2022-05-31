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