#include <iostream>

#include "Block.h"
#include "Statement.h"

std::ostream& operator<<(std::ostream& os, Block const& block)
{
	os << block.name << " {\n";
	for (Statement const& s : block.statements)
	{
		os << s << '\n';
	}
	os << "}";
	return os;
}