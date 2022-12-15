#include <thread> // for sleeping
#include <chrono>
#include <iostream>
#include <string>

#include "common/Tryte.h"
#include "common/constants.h"
#include "CPU/CPU.h"
#include "Memory/MemoryBlock.h"

CPU::CPU(MemoryBlock& memory) :
	memory_{memory}
{
	// CPU is switched off
	on_ = false;
	// debug mode off by default
	debug_mode_ = false;
}

void CPU::turn_on()
{
	on_ = true;
	asleep_ = false;
}

void CPU::turn_off()
{
	on_ = false;
}

void CPU::sleep()
{
	asleep_ = true;
}

void CPU::wake()
{
	asleep_ = false;
}

bool CPU::is_asleep()
{
	return asleep_;
}

bool CPU::is_on()
{
	return on_;
}

void CPU::cycle()
{
	fetch();
	decode_and_execute();
	cycles_++;
}

void CPU::run()
{
	this->turn_on();
	while (on_)
	{
		auto frame_start = clock_.now();
		if (!asleep_)
		{
			if (debug_mode_)
			{
				debug();
			}
			else
			{
				cycle();
				if (cycles_ % cycles_per_frame_ == 0)
				{
					// if we're early, wait until 1/60 of a second has passed before continuing
					std::this_thread::sleep_until(frame_start + frame_duration_);
					frame_start = clock_.now();
				}
				dump();
			}
		}
	}
}

void CPU::dump()
{
	std::cout << "Program counter: " << pc_ << '\n';
	std::cout << "Last instruction: " << instr_ << '\n';
	std::cout << "Stack pointer: " << sp_ << '\n';
	std::cout << "Memory bank: " << memory_.bank() << '\n';

	std::cout << "---------REGISTERS---------\n";
	std::cout << "A : " << regs_[1] << " B : " << regs_[2] << " C : " << regs_[3] << '\n';
	std::cout << "D : " << regs_[4] << " E : " << regs_[5] << " F : " << regs_[6] << '\n';
	std::cout << "G : " << regs_[7] << " H : " << regs_[8] << " I : " << regs_[9] << '\n';

	std::cout << "-----------STACK-----------\n";
	dump_stack();
}

void CPU::dump_stack()
{
	Tryte stack_start = sp_;
	while (stack_start != constants::STACK_BOTTOM)
	{
		std::cout << stack_start << ": " << memory_[stack_start] << '\n';
		stack_start += 1;
	}
}

void CPU::crash(std::string const& err_msg)
{
	std::cerr << "TRIUMPH exception: " << err_msg << '\n';
	dump();
	halt();
}

bool& CPU::debug_mode()
{
	return debug_mode_;
}

void CPU::fetch()
{
	instr_ = memory_[pc_];
}

void CPU::decode_and_execute()
{
	int8_t high = instr_.get_high();
	int8_t mid = instr_.get_mid();
	int8_t low = instr_.get_low();

	if (high == 10)
	{
		// found a jump instruction
		decode_and_execute_jump(mid, low);
		return;
	}

	switch (high)
	{
	case -13:
		// M - no Tryte register instructions
		switch (mid)
		{
		case -13:
			// MM* - BREAK
			enter_debug();
			instr_size_ = 1;
			break;
		case 0:
			// M0* - SLEEP
			sleep();
			instr_size_ = 1;
			break;
		case 6:
			// Mf* t9 - PUSH t9
			push(memory_[pc_ + 1]);
			instr_size_ = 2;
			break;
		case 10:
			// Mj* t9 - BANK t9
			set_bank(memory_[pc_ + 1]);
			instr_size_ = 2;
			break;
		default:
			crash("Unrecognised instruction");
			break;
		}
		break;
	case -11:
		// KXY - SH X, Y
		trit_shift(regs_[mid], regs_[low]);
		instr_size_ = 1;
		break;
	case -8:
		// HX(t3) - SH X, t3
		trit_shift(regs_[mid], constants::int_to_tryte.at(low));
		instr_size_ = 1;
		break;
	case -7:
		// GXY - SBB X, Y
		sub_with_borrow(regs_[mid], regs_[low]);
		instr_size_ = 1;
		break;
	case -5:
		// EXY - SUB X, Y
		sub(regs_[mid], regs_[low]);
		instr_size_ = 1;
		break;
	case -4:
		// DXY - SAVE X, [Y]
		save(regs_[mid], regs_[low]);
		instr_size_ = 1;
		break;
	case -2:
		// BXY - OR X, Y
		trit_or(regs_[mid], regs_[low]);
		instr_size_ = 1;
		break;
	case -1:
		// AXY - SWAP X, Y
		swap(regs_[mid], regs_[low]);
		instr_size_ = 1;
		break;
	case 0:
		// 0XY - HALT - maybe debugging instructions here?
		halt();
		instr_size_ = 1;
		break;
	case 1:
		// aXY - SET X, Y
		set(regs_[mid], regs_[low]);
		instr_size_ = 1;
		break;
	case 2:
		// bXY - AND X, Y
		trit_and(regs_[mid], regs_[low]);
		instr_size_ = 1;
		break;
	case 3:
		// cXY - CMP X, Y
		compare(regs_[mid], regs_[low]);
		instr_size_ = 1;
		break;
	case 4:
		// dXY - LOAD X, [Y]
		load(regs_[mid], regs_[low]);
		instr_size_ = 1;
		break;
	case 5:
		// eXY - ADD X, Y
		add(regs_[mid], regs_[low]);
		instr_size_ = 1;
		break;
	case 7:
		// gXY - ADC X, Y
		add_with_carry(regs_[mid], regs_[low]);
		instr_size_ = 1;
		break;
	case 8:
		// hXY - SH X, Y
		trit_shift(regs_[mid], regs_[low]);
		instr_size_ = 1;
		break;
	case 11:
		// kXY - STAR X, Y
		trit_star(regs_[mid], regs_[low]);
		instr_size_ = 1;
		break;
	case 13:
		// m - single Tryte register operations
		switch (mid)
		{
		case -13:
			// mMX - FLIP X
			flip(regs_[low]);
			instr_size_ = 1;
			break;
		case -12:
			// mLX - DEC X
			decrement(regs_[low]);
			instr_size_ = 1;
			break;
		case -10:
			// mJX - GANK X
			get_bank(regs_[low]);
			instr_size_ = 1;
			break;
		case -7:
			// mGX t9 - SBB X, t9
			sub_with_borrow(regs_[low], memory_[pc_ + 1]);
			instr_size_ = 2;
			break;
		case -6:
			// mFX - POP X
			pop(regs_[low]);
			instr_size_ = 1;
			break;
		case -5:
			// mEX t9 - SUB X, t9
			sub(regs_[low], memory_[pc_ + 1]);
			instr_size_ = 2;
			break;
		case -4:
			// mDX $Y - SAVE X, $Y
			save(regs_[low], memory_[pc_ + 1]);
			instr_size_ = 2;
			break;
		case -3:
			// mCX t9 - CPZ X
			compare(regs_[low], 0);
			instr_size_ = 1;
			break;
		case -2:
			// mBX t9 - OR X, t9
			trit_or(regs_[low], memory_[pc_ + 1]);
			instr_size_ = 2;
			break;
		case 0:
			// m0X - ZERO X
			zero(regs_[low]);
			instr_size_ = 1;
			break;
		case 1:
			// maX t9 - SET X, t9
			set(regs_[low], memory_[pc_ + 1]);
			instr_size_ = 2;
			break;
		case 2:
			// mbX t9 - AND X, t9
			trit_and(regs_[low], memory_[pc_ + 1]);
			instr_size_ = 2;
			break;
		case 3:
			// mcX t9 - CMP X, t9
			compare(regs_[low], memory_[pc_ + 1]);
			instr_size_ = 2;
			break;
		case 4:
			// mdX $y - LOAD X, $Y
			load(regs_[low], memory_[pc_ + 1]);
			instr_size_ = 2;
			break;
		case 5:
			// meX t9 - ADD X, t9
			add(regs_[low], memory_[pc_ + 1]);
			instr_size_ = 2;
			break;
		case 6:
			// mfX - PUSH X
			push(regs_[low]);
			instr_size_ = 1;
			break;
		case 7:
			// mgX t9 - ADC X, t9
			add_with_carry(regs_[low], memory_[pc_ + 1]);
			instr_size_ = 2;
			break;
		case 10:
			// mjX - BANK X
			set_bank(regs_[low]);
			instr_size_ = 1;
			break;
		case 11:
			// mkX t9 - STAR X, t9
			trit_star(regs_[low], memory_[pc_ + 1]);
			instr_size_ = 2;
			break;
		case 12:
			// mlX - INC X
			increment(regs_[low]);
			instr_size_ = 1;
			break;
		case 13:
			// mmX - PEEK X
			peek(regs_[low]);
			instr_size_ = 1;
			break;
		default:
			crash("Unrecongised instruction");
			break;
		}
		break;
	default:
		crash("Unrecognised instruction");
		break;
	}
	// proceed to next instruction
	pc_ += instr_size_;
}