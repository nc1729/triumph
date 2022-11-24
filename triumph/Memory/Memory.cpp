#include <vector>
#include <string>
#include <array>
#include <stdexcept>
#include <iostream>

#include "common/Tryte.h"
#include "IO/Disk.h"
#include "Memory/Memory.h"
#include "Memory/Bank.h"

Tryte const Memory::BANK = 9839;
Tryte const Memory::SP = 9840;
Tryte const Memory::PC = 9841;
Tryte const Memory::BANK_START = -9841;
Tryte const Memory::STACK_BOTTOM = 9829;
Tryte const Memory::STACK_TOP = 9113;
Tryte const Memory::BANK_END = -3280;

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

Tryte const& Memory::bank() const
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

void Memory::dump()
{
	int64_t const ROW_LENGTH = 27;
	Tryte row_start = -9841;
	Tryte row_end = -9841 + ROW_LENGTH - 1;

	// print bank number
	std::cout << "TRIUMPH memory dump\n";
	std::cout << "Bank number: " << bank();

	// draw top row
	std::cout << "      M   L   K   J   I   H   G   F   E   D   C   B   A   0   a   b   c";
	std::cout << "   d   e   f   g   h   i   j   k   l   m     \n";

	// dump contents of memory
	for (int64_t row = 0; row < 19683 / 27; row++)
	{
		std::cout << row_start << ": ";
		for (int64_t j = 0; j < ROW_LENGTH; j++)
		{
			std::cout << (*this)[row_start + j] << ' ';
		}
		std::cout << row_end << '\n';

		row_start = row_end;
		row_end += ROW_LENGTH;
	}
	std::cout << '\n';
}