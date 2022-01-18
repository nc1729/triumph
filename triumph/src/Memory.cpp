#include <vector>
#include <array>

#include "Memory.h"
#include "Bank.h"
#include "Tryte.h"

Memory::Memory(size_t const number_of_banks)
{
	// initialise memory banks
	_banks.resize(number_of_banks);

	// initialise local memory
	_local.fill(0);

	// write boot code to memory
	init();

}

Tryte& Tryte::operator[](int64_t const addr)
{
	if (addr < -9841 + 6561)
	{
		// accessing memory bank
	}
	else if (addr > 9841 - 365)
	{
		// attempted to access reserved memory
	}
	else
	{
		return _local[addr + 9841 - 6561];
	}
}