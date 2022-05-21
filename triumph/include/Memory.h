#pragma once

#include <cstdint>
#include <array>
#include <vector>

#include "BankManager.h"
#include "Tryte.h"

class Memory
{
private:
	BankManager banks_;
	std::array<Tryte, 6561 * 2> local_;
	// write boot code to memory
	//void init();

public:
	Memory(std::vector<Bank*> const& banks);

	Tryte& operator[](int64_t const addr);
	Tryte const& operator[](int64_t const addr) const;

	// $mmk - current memory bank
	static int64_t const BANK = 9839;
	// $mml - stack pointer
	static int64_t const SP = 9840;
	// $mmm - program counter
	static int64_t const PC = 9841;
	// $Emm - bank memory end
	static int64_t const BANK_END = (-9841 + 6561);
	// $m00 - reserved memory start
	static int64_t const RESERVED_START = (9841 - 365);

	// public access to current bank
	Tryte& bank();
	// public access to stack pointer
	Tryte& sp();
	// public access to program counter (for jump instructions)
	Tryte& pc();

	// dump a program into an address and move the program counter to that address, ready to run
	void load_program(int64_t addr, std::vector<Tryte> const& program);

	// add a bank (if device 'plugged in')
	void add_bank(Bank* new_bank_ptr);
};
