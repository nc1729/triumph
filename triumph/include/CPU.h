#pragma once

#include "Tryte.h"
#include "Memory.h"

#include <string>
#include <array>
#include <chrono>

class CPU
{
public:
	// constructor
	CPU(Memory& memory);
	// turn CPU on
	void turn_on();
	// turn CPU off
	void turn_off();
	// return CPU status
	bool is_on();
	// perform a single CPU fetch/decode/execute cycle
	void cycle();
	// while CPU is on, run fetch/decode/execute cycle
	void run();
	// dump contents of registers and other things to stdout
	void dump();
	// on crash, dump contents of registers and other things to stderr
	void crash_dump(std::string const& err_msg);


private:
	/*
	registers
	*/
	// define registers and aliases for them
	std::array<Tryte, 10> regs_ = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	// store the current instruction at position 0 - other regs follow in positions 1-9
	Tryte& instr_ = regs_[0];

	/*
	flag variables
	*/
	// mark flags register
	Tryte& flags_ = regs_[6];
	int8_t& COMPARE_FLAG = flags_[0];
	int8_t& CARRY_FLAG = flags_[1];
	int8_t& SIGN_FLAG = flags_[2];
	int8_t& STACK_FLAG = flags_[3];


	/*
	memory variables
	*/
	// reference to memory array
	Memory& memory_;
	// program counter
	Tryte& pc_ = memory_.pc();
	// stack pointer
	Tryte& sp_ = memory_.sp();
	// current bank
	Tryte& bank_ = memory_.bank();

	/*
	clock variables
	*/
	// system clock
	std::chrono::system_clock clock_;
	// precise time the clock was started
	std::chrono::time_point<std::chrono::steady_clock> boot_time_;
	// time updated every time max_frequency cycles elapses
	std::chrono::time_point<std::chrono::system_clock> second_start_;
	// number of cycles elasped so far
	int64_t cycles_;
	// maximum number of cycles per second
	size_t max_frequency_;
	// instruction size, used for jumping
	size_t instr_size_;
	// CPU is being throttled by the emulator
	bool throttled_;

	bool print = false;

	/*
	CPU variables and private functions
	*/
	// on/off switch
	bool on_;

	// fetch the Tryte at the instruction pointer and set it as current instruction
	void fetch();

	// decode the current instruction and execute it
	void decode_and_execute();

	/*
	CPU operations
	*/

	// all variations of jump instructions
	void decode_and_execute_jump(int8_t const mid, int8_t const low);

	// load Tryte from memory 
	void load(Tryte const& x, Tryte& y);

	// save Tryte to memory
	void save(Tryte& x, Tryte const& y);

	// set memory bank to Tryte value
	void set_bank(Tryte const& x);

	// get memory bank
	void get_bank(Tryte& x);

	// set register equal to register/immediate
	void set(Tryte& x, Tryte const& y);

	// swap registers
	void swap(Tryte& x, Tryte& y);

	// push Tryte to stack
	void push(Tryte const& x);

	// pop Tryte off stack
	void pop(Tryte& x);

	// peek Tryte from top of stack
	void peek(Tryte& x);

	// register += register/immediate
	void add(Tryte& x, Tryte const& y);

	// add with carry
	void add_with_carry(Tryte& x, Tryte const& y);

	// subtract Trytes
	void sub(Tryte& x, Tryte const& y);

	// sub with borrow
	void sub_with_borrow(Tryte& x, Tryte const& y);

	// tritwise shift
	void trit_shift(Tryte& x, Tryte const& y);

	// increment Tryte
	void increment(Tryte& x);

	// decrement Tryte
	void decrement(Tryte& x);

	// flip Tryte (multiply by -1; tritwise NOT operator)
	void flip(Tryte& x);

	// set Tryte to zero
	void zero(Tryte& x);

	// compare Trytes
	void compare(Tryte const& x, Tryte const& y);

	// apply tritwise AND operator
	void trit_and(Tryte& x, Tryte const& y);

	// apply tritwise OR operator
	void trit_or(Tryte& x, Tryte const& y);

	// apply tritwise STAR operator
	void trit_star(Tryte& x, Tryte const& y);

	// no operation
	void nop();

	// halt and catch fire
	void halt();

	// ternary jump instruction
	void jump(Tryte const& x, Tryte const& y, Tryte const& z);

	// store program counter on stack and jump to x
	void jump_and_store(Tryte const& x);

	// pop from stack and jump to popped address
	void pop_and_jump();

	/* for debugging */
	// push Tryte to stdout
	void show(Tryte const& x);

	// fetch Tryte from stdin
	void tell(Tryte& x);
};