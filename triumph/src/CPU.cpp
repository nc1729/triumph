#include "CPU.h"
#include "Memory.h"
#include "Tryte.h"

#include <thread> // for sleeping
#include <chrono>
#include <iostream>
#include <string>

CPU::CPU(Memory& memory) :
	memory_{memory}
{
	// initialise clock variables
	cycles_ = 0;
	max_frequency_ = 100000;
	throttled_ = false;

	// CPU is switched off
	on_ = false;

	// registers/stack pointer/etc are set in boot code in Memory class
}

void CPU::turn_on()
{
	on_ = true;
}

void CPU::turn_off()
{
	on_ = false;
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
		cycle();
		// moderate processor speed
		if (cycles_ % max_frequency_ == 0)
		{
			if (throttled_)
			{
				if ((clock_.now() - second_start_) < std::chrono::seconds{ 1 })
				{
					// we are going fast enough to hit the max frequency, unthrottle
					throttled_ = false;
				}
			}
			else
			{
				if ((clock_.now() - second_start_) < std::chrono::seconds{ 1 })
				{
					// pause until a second has passed, then move on to next cycle
					std::this_thread::sleep_until(second_start_ + std::chrono::seconds{ 1 });
				}
				else
				{
					// max_frequency_ cycles has taken longer than a second - throttle
					throttled_ = true;
				}
			}
			second_start_ = clock_.now();
		}
		//dump();
	}
}

void CPU::dump()
{
	std::cout << "Program counter: " << pc_ << '\n';
	std::cout << "Last instruction: " << instr_ << '\n';
	std::cout << "Stack pointer: " << sp_ << '\n';

	std::cout << "---------REGISTERS---------\n";
	std::cout << "A : " << regs_[1] << " B : " << regs_[2] << " C : " << regs_[3] << '\n';
	std::cout << "D : " << regs_[4] << " E : " << regs_[5] << " F : " << regs_[6] << '\n';
	std::cout << "G : " << regs_[7] << " H : " << regs_[8] << " I : " << regs_[9] << '\n';
}

void CPU::crash_dump(std::string const& err_msg)
{
	std::cerr << "TRIUMPH exception: " << err_msg << '\n';
	dump();
}

void CPU::fetch()
{
	instr_ = memory_[pc_];
}

void CPU::decode_and_execute()
{
	int8_t high = Tryte::get_high(instr_);
	int8_t mid = Tryte::get_mid(instr_);
	int8_t low = Tryte::get_low(instr_);

	switch (high)
	{

	case -13:
		// M - no Tryte register instructions
		switch (mid)
		{
		case -13:
			// MM. - HALT
			halt();
			instr_size_ = 1;
			break;
		case 0:
			// M0. - NOP
			nop();
			instr_size_ = 1;
			break;
		case 6:
			// Mf. t9 - PUSH t9
			push(memory_[pc_ + 1]);
			instr_size_ = 2;
			break;
		case 10:
			// Mj. t9 - BANK t9
			set_bank(memory_[pc_ + 1]);
			instr_size_ = 2;
			break;
		default:
			crash_dump("Unrecognised instruction");
			halt();
			break;
		}
		break;
	case -12:
		// L - console input/output
		switch (mid)
		{
			case -1:
			    // LAX - TELL X
				tell(regs_[low]);
				instr_size_ = 1;
				break;
			case 1:
				// LaX - SHOW X
				show(regs_[low]);
				instr_size_ = 1;
				break;
			default:
				crash_dump("Unrecognised instruction");
				halt();
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
		trit_shift(regs_[mid], Tryte(low));
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
		// dXY - LOAD [X], Y
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
	case 10:
		// j - jump instructions
		switch (mid)
		{
		case -9:
			// jIX - JPNZ [X]
			jump_if_not_zero(memory_[regs_[low]]);
			instr_size_ = 1;
			break;
		case -1:
			// jAX - JPN [X]
			jump_if_neg(memory_[regs_[low]]);
			instr_size_ = 1;
			break;
		case 1:
			// jaX - JPP [X]
			jump_if_pos(memory_[regs_[low]]);
			instr_size_ = 1;
			break;
		case 9:
			// jiX - JPZ [X]
			jump_if_zero(memory_[regs_[low]]);
			instr_size_ = 1;
			break;
		case 10:
			// jj - jump instructions with addresses
			switch (low)
			{
			case -9:
				// jjI - JPNZ $X
				jump_if_not_zero(memory_[pc_ + 1]);
				instr_size_ = 2;
				break;
			case -6:
				// jjF - PJP
				pop_and_jump();
				instr_size_ = 1;
				break;
			case -1:
				// jjA - JPN $X
				jump_if_neg(memory_[pc_ + 1]);
				instr_size_ = 2;
				break;
			case 0:
				// jj0 - JP $X
				jump(memory_[pc_ + 1]);
				instr_size_ = 2;
				break;
			case 1:
				// jja - JPP $X
				jump_if_pos(memory_[pc_ + 1]);
				instr_size_ = 2;
				break;
			case 6:
				// jjf - JPS $X
				jump_and_store(memory_[pc_ + 1]);
				instr_size_ = 2;
				break;
			case 9:
				// jji - JPZ $X
				jump_if_zero(memory_[pc_ + 1]);
				instr_size_ = 2;
				break;
			case 10:
				// jjj - TJP $X, $Y, $Z
				ternary_jump(memory_[pc_ + 1], memory_[pc_ + 2], memory_[pc_ + 3]);
				instr_size_ = 4;
				break;
			default:
				crash_dump("Unrecognised instruction");
				halt();
				break;
			}
			break;
		default:
			crash_dump("Unrecognised instruction");
			halt();
			break;
		}
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
			// mdY $X - LOAD $X, Y
			load(memory_[pc_ + 1], regs_[low]);
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
			crash_dump("Unrecongised instruction");
			halt();
			break;
		}
		break;
	default:
		crash_dump("Unrecognised instruction");
		halt();
		break;
	}
	if (JUMP_FLAG == 0)
	{
		pc_ += instr_size_;
	}
	else
	{
		JUMP_FLAG = 0;
	}

}