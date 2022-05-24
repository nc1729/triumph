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

	Tryte& operator[](int64_t const addr);
	Tryte const& operator[](int64_t const addr) const;

	// $mmk - current memory bank
	static int64_t constexpr BANK = 9839;
	// $mml - stack pointer
	static int64_t constexpr SP = 9840;
	// $mmm - program counter
	static int64_t constexpr PC = 9841;
	// $MMM - bank memory start
	static int64_t constexpr BANK_START = -9841;
	// $Emm - bank memory end
	static int64_t constexpr BANK_END = (-9841 + 6561);
	// $mmk-mmm - reserved memory
	static int64_t constexpr RESERVED_START = (9841 - 3);

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
