#include <vector>
#include <string>
#include <array>
#include <stdexcept>

#include "Memory.h"
#include "Bank.h"
#include "Disk.h"
#include "Tryte.h"

Tryte const Memory::BANK = 9839;
Tryte const Memory::SP = 9840;
Tryte const Memory::PC = 9841;
Tryte const Memory::BANK_START = -9841;
Tryte const Memory::STACK_BOTTOM = 9828;
Tryte const Memory::STACK_TOP = 9113;
Tryte const Memory::BANK_END = -3281;

Memory::Memory(std::vector<Bank*> const& banks, std::vector<Disk>& disks) : banks_(banks)
{
	// initialise local memory
	local_.fill(0);

	// disks occupy memory banks 1 to ... (defined by Disk constructors)
	for (auto& disk : disks)
	{
		banks_.add_bank(&disk.buffer);
	}
}

void Memory::add_bank(Bank* new_bank_ptr)
{
	banks_.add_bank(new_bank_ptr);
}

void Memory::load_program(Tryte const& addr, std::vector<Tryte> const& program)
{
	this->sp() = Memory::STACK_BOTTOM;
	this->pc() = addr;

	for (int64_t i = 0; i < static_cast<int64_t>(program.size()); ++i)
	{
		(*this)[addr + i] = program[i];
	}
}

Tryte& Memory::operator[](Tryte const& addr)
{
	if (addr < Memory::BANK_END)
	{
		// accessing memory bank
		return banks_(Tryte::get_int(this->bank()), addr);
	}
	else
	{
		return local_[addr - Memory::BANK_END];
	}
}

Tryte const& Memory::operator[](Tryte const& addr) const
{
	if (addr < Memory::BANK_END)
	{
		// accessing memory bank
		return banks_(Tryte::get_int(this->bank()), addr);
	}
	else
	{
		return local_[addr - Memory::BANK_END];
	}
}

Tryte& Memory::bank()
{
	return local_[Memory::BANK + 9841 - 6561];
}

Tryte& Memory::sp()
{
	return local_[Memory::SP + 9841 - 6561];
}

Tryte& Memory::pc()
{
	return local_[Memory::PC + 9841 - 6561];
}
