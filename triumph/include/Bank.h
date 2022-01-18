#pragma once

#include <vector>

#include "Tryte.h"
class Bank
{
private:
	std::vector<Tryte> _bank;

public:
	Bank(size_t const size = 6561);

	Tryte& operator[](size_t const i);
	Tryte const& operator[](size_t const i) const;
};