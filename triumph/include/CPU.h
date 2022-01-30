#pragma once

#include "Tryte.h"
#include "Memory.h"
#include "PortManager.h"

#include <string>
#include <array>
#include <chrono>

class CPU
{
public:
	// constructor
	CPU(Memory& memory, PortManager& devices);
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
	// dump contents of registers and other things to stderr
	void dump(std::string const& err_msg);


private:
	/*
	registers
	*/
	// define registers and aliases for them
	std::array<Tryte, 10> _regs;
	Tryte& _instr = _regs[0]; // store the current instruction here
	// start registers at index 1 - allows 'a' to match _a, 'b' to match _b etc
	Tryte& _a = _regs[1];
	Tryte& _b = _regs[2];
	Tryte& _c = _regs[3];
	Tryte& _d = _regs[4];
	Tryte& _e = _regs[5];
	Tryte& _f = _regs[6];
	Tryte& _g = _regs[7];
	Tryte& _h = _regs[8];
	Tryte& _i = _regs[9];

	/*
	flag variables
	*/
	// mark flags register
	Tryte& _flags = _f;
	int8_t& COMPARE_FLAG = _flags[0];
	int8_t& CARRY_FLAG = _flags[1];
	int8_t& SIGN_FLAG = _flags[2];
	int8_t& STACK_FLAG = _flags[3];


	/*
	memory variables
	*/
	// reference to memory array
	Memory& _memory;
	// program counter
	Tryte& _pc = _memory.pc();
	// stack pointer
	Tryte& _sp = _memory.sp();
	// current bank
	Tryte& _bank = _memory.bank();
	// current port
	Tryte& _port = _memory.port();

	/*
	external ports
	*/
	PortManager& _devices;

	/*
	clock variables
	*/
	// system clock
	std::chrono::system_clock _clock;
	// precise time the clock was started
	std::chrono::time_point<std::chrono::steady_clock> _boot_time;
	// time updated every time max_frequency cycles elapses
	std::chrono::time_point<std::chrono::system_clock> _second_start;
	// number of cycles elasped so far
	int64_t _cycles;
	// maximum number of cycles per second
	size_t _max_frequency;
	// CPU is being throttled by the emulator
	bool _throttled;

	/*
	CPU variables and private functions
	*/
	// on/off switch
	bool _on;

	// fetch the Tryte at the instruction pointer and set it as current instruction
	void fetch();

	// decode the current instruction and execute it
	void decode_and_execute();

	/*
	CPU operations
	*/

	// load Tryte from memory 
	void load(Tryte const& x, Tryte& y);

	// save Tryte to memory
	void save(Tryte& x, Tryte const& y);

	// set memory bank to Tryte value
	void set_bank(Tryte const& x);

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

	// receive Tryte from current port
	void in(Tryte& x);

	// send Tryte out to current port
	void out(Tryte const& x);

	// set current port to Tryte value
	void set_port(Tryte const& x);

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

	// jump if zero
	void jump_if_zero(Tryte const& x);

	// jump if not zero
	void jump_if_not_zero(Tryte const& x);

	// jump if positive
	void jump_if_pos(Tryte const& x);

	// jump if negative
	void jump_if_neg(Tryte const& x);

	// jump unconditionally
	void jump(Tryte const& x);

	// jump and store
	void jump_and_store(Tryte const& x);

	// pop and jump
	void pop_and_jump();

	// ternary jump
	void ternary_jump(Tryte const& x, Tryte const& y, Tryte const& z);
};