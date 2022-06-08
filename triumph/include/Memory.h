#pragma once

#include <cstdint>
#include <array>
#include <vector>

#include "BankManager.h"
#include "Tryte.h"
#include "Disk.h"

class Memory
{
private:
	BankManager banks_;
	std::array<Tryte, 6561 * 2> local_;
	// write boot code to memory
	//void init();

public:
	Memory(std::vector<Bank*> const& banks, std::vector<Disk>& disks);

	Tryte& operator[](Tryte const& addr);
	Tryte const& operator[](Tryte const& addr) const;

	// $mmk - current memory bank
	static Tryte const BANK;
	// $mml - stack pointer
	static Tryte const SP;
	// $mmm - program counter
	static Tryte const PC;
	// $MMM - bank memory start
	static Tryte const BANK_START;
	// $mm0 - stack bottom
	static Tryte const STACK_BOTTOM;
	// $mMM - stack top
	static Tryte const STACK_TOP;
	// $Emm - bank memory end
	static Tryte const BANK_END;

	// public access to current bank
	Tryte& bank();
	Tryte const& bank() const;
	// public access to stack pointer
	Tryte& sp();
	// public access to program counter (for jump instructions)
	Tryte& pc();

	// dump a program into an address and move the program counter to that address, ready to run
	void load_program(Tryte const& addr, std::vector<Tryte> const& program);

	// add a bank (if device 'plugged in')
	void add_bank(Bank* new_bank_ptr);
};
