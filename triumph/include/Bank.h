#pragma once

#include <vector>

#include "Tryte.h"

class Bank
{
private:
	std::vector<Tryte> bank_;

public:
	// each bank has a number used to address it
	// switch to a bank using BANK instruction
	int64_t number;
	Bank(int64_t const number, size_t const size = 6561);

	Tryte& operator[](size_t const i);
	Tryte const& operator[](size_t const i) const;
};
