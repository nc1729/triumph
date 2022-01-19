#include <vector>

#include "Bank.h"
#include "Tryte.h"

Bank::Bank(int64_t const number, size_t const size) : number{number}
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
