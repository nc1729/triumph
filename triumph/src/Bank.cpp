#include <vector>

#include "Bank.h"
#include "Tryte.h"

Bank::Bank(size_t const size = 6561)
{
	// initialise all Trytes in memory bank to zero
	_bank.resize(size);
}

Tryte& Bank::operator[](size_t const i)
{
	return _bank[i];
}

Tryte const& Bank::operator[](size_t const i) const
{
	return _bank[i];
}