#pragma once

#include <cstdint>
#include <array>

#include "BankManager.h"
#include "Tryte.h"

class Memory
{
private:
	BankManager _banks;
	std::array<Tryte, 6561 * 2> _local;
	// write boot code to memory
	void init();

public:
	Memory(size_t const number_of_banks, std::vector<Bank*> const& bank_ptrs);

	Tryte& operator[](int64_t const addr);
	Tryte const& operator[](int64_t const addr) const;

	// $mmj - current port
	static int64_t const PORT = 9838;
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

	// public access to current port
	Tryte& port();
	// public access to current bank
	Tryte& bank();
	// public access to stack pointer
	Tryte& sp();
	// public access to program counter (for jump instructions)
	Tryte& pc();
};