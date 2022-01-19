#include <vector>
#include <array>

#include "Memory.h"
#include "Bank.h"
#include "Tryte.h"

Memory::Memory(size_t const number_of_banks, std::vector<Bank*> const& bank_ptrs) : _banks(bank_ptrs)
{
	// initialise local memory
	_local.fill(0);

	// write boot code to memory
	init();

}

Tryte& Memory::operator[](int64_t const addr)
{
	if (addr < -9841 + 6561)
	{
		// accessing memory bank
		// first get current bank
		int64_t current_bank = Tryte::get_int((*this)[Memory::BANK]);
		return _banks[current_bank][]
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