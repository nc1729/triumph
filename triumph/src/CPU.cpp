#include "CPU.h"
#include "Memory.h"
#include "PortManager.h"
#include "Tryte.h"

#include <thread> // for sleeping
#include <chrono>
#include <iostream>
#include <string>

CPU::CPU(Memory& memory, PortManager& devices) :
	_memory{memory}, _devices{devices}
{
	// initialise clock variables
	_cycles = 0;
	_max_frequency = 100000;
	_throttled = false;

	// CPU is switched off
	_on = false;

	// registers/stack pointer/etc are set in boot code in Memory class
}

void CPU::turn_on()
{
	_on = true;
}

void CPU::turn_off()
{
	_on = false;
}

bool CPU::is_on()
{
	return _on;
}

void CPU::cycle()
{
	fetch();
	decode_and_execute();
	_cycles++;
}

void CPU::run()
{
	while (_on)
	{
		cycle();
		// moderate processor speed
		if (_cycles % _max_frequency == 0)
		{
			if (_throttled)
			{
				if ((_clock.now() - _second_start) < std::chrono::seconds{ 1 })
				{
					// we are going fast enough to hit the max frequency, unthrottle
					_throttled = false;
				}
			}
			else
			{
				if ((_clock.now() - _second_start) < std::chrono::seconds{ 1 })
				{
					// pause until a second has passed, then move on to next cycle
					std::this_thread::sleep_until(_second_start + std::chrono::seconds{ 1 });
				}
				else
				{
					// _max_frequency cycles has taken longer than a second - throttle
					_throttled = true;
				}
			}
			_second_start = _clock.now();
		}
	}
}

void CPU::dump(std::string const& err_msg)
{
	std::cerr << "TRIUMPH exception: " << err_msg << '\n';
	std::cerr << "Instruction pointer: " << _memory[_pc] << '\n';
	std::cerr << "Stack pointer: " << _memory[_sp] << '\n';
	std::cerr << "---------REGISTERS---------\n";
	std::cerr << "A : " << _a << " B : " << _b << " C : " << _c << '\n';
	std::cerr << "D : " << _d << " E : " << _e << " F : " << _f << '\n';
	std::cerr << "G : " << _g << " H : " << _h << " I : " << _i << '\n';
}

void CPU::fetch()
{
	_instr = _memory[_pc];
}

void CPU::decode_and_execute()
{
	int8_t high = Tryte::get_high(_instr);
	int8_t mid = Tryte::get_mid(_instr);
	int8_t low = Tryte::get_low(_instr);

	switch (high)
	{

	case -13:
		// M - no Tryte register instructions
		switch (mid)
		{
		case -13:
			// MM. - HALT
			halt();
			_pc += 1;
			break;
		case -10:
			// MJ. t9 - PORT t9
			set_port(_memory[_pc + 1]);
			_pc += 2;
			break;
		case 0:
			// M0. - NOP
			nop();
			_pc += 1;
			break;
		case 6:
			// Mf. t9 - PUSH t9
			push(_memory[_pc + 1]);
			_pc += 2;
			break;
		case 10:
			// Mj. t9 - BANK t9
			set_bank(_memory[_pc + 1]);
			_pc += 2;
			break;
		default:
			dump("Unrecognised instruction");
			halt();
			break;
		}
		break;
	case -7:
		// GXY - SBB X, Y
		sub_with_borrow(_regs[mid], _regs[low]);
		_pc += 1;
		break;
	case -5:
		// EXY - SUB X, Y
		sub(_regs[mid], _regs[low]);
		_pc += 1;
		break;
	case -4:
		// DXY - SAVE X, [Y]
		save(_regs[mid], _regs[low]);
		_pc += 1;
		break;
	case -3:
		// CXY - SH X, Y
		trit_shift(_regs[mid], _regs[low]);
		_pc += 1;
		break;
	case -2:
		// BXY - OR X, Y
		trit_or(_regs[mid], _regs[low]);
		_pc += 1;
		break;
	case -1:
		// AXY - SWAP X, Y
		swap(_regs[mid], _regs[low]);
		_pc += 1;
		break;
	case 1:
		// aXY - SET X, Y
		set(_regs[mid], _regs[low]);
		_pc += 1;
		break;
	case 2:
		// bXY - AND X, Y
		trit_and(_regs[mid], _regs[low]);
		_pc += 1;
		break;
	case 3:
		// cXY - CMP X, Y
		compare(_regs[mid], _regs[low]);
		_pc += 1;
		break;
	case 4:
		// dXY - LOAD [X], Y
		load(_regs[mid], _regs[low]);
		_pc += 1;
		break;
	case 5:
		// eXY - ADD X, Y
		add(_regs[mid], _regs[low]);
		_pc += 1;
		break;
	case 7:
		// gXY - ADC X, Y
		add_with_carry(_regs[mid], _regs[low]);
		_pc += 1;
		break;
	case 10:
		// j - jump instructions
		switch (mid)
		{
		case -9:
			// jIX - JPNZ [X]
			jump_if_not_zero(_memory[_regs[low]]);
			_pc += 1;
			break;
		case -1:
			// jAX - JPN [X]
			jump_if_neg(_memory[_regs[low]]);
			_pc += 1;
			break;
		case 1:
			// jaX - JPP [X]
			jump_if_pos(_memory[_regs[low]]);
			_pc += 1;
			break;
		case 9:
			// jiX - JPZ [X]
			jump_if_zero(_memory[_regs[low]]);
			_pc += 1;
			break;
		case 10:
			// jj - jump instructions with addresses
			switch (low)
			{
			case -9:
				// jjI - JPNZ $X
				jump_if_not_zero(_memory[_pc + 1]);
				_pc += 1;
				break;
			case -6:
				// jjF - PJP
				pop_and_jump();
				_pc += 1;
				break;
			case -1:
				// jjA - JPN $X
				jump_if_neg(_memory[_pc + 1]);
				_pc += 1;
				break;
			case 0:
				// jj0 - JP $X
				jump(_memory[_pc + 1]);
				_pc += 1;
				break;
			case 1:
				// jja - JPP $X
				jump_if_pos(_memory[_pc + 1]);
				_pc += 1;
				break;
			case 6:
				// jjf - JPS $X
				jump_and_store(_memory[_pc + 1]);
				_pc += 1;
				break;
			case 9:
				// jji - JPZ $X
				jump_if_zero(_memory[_pc + 1]);
				_pc += 1;
				break;
			case 10:
				// jjj - TJP $X, $Y, $Z
				ternary_jump(_memory[_pc + 1], _memory[_pc + 2], _memory[_pc + 3]);
				_pc += 1;
				break;
			default:
				dump("Unrecognised instruction");
				halt();
				break;
			}
			break;
		default:
			dump("Unrecognised instruction");
			halt();
			break;
		}
		break;
	case 11:
		// kXY - STAR X, Y
		trit_star(_regs[mid], _regs[low]);
		_pc += 1;
		break;
	case 13:
		// m - single Tryte register operations
		switch (mid)
		{
		case -13:
			// mMX - FLIP X
			flip(_regs[low]);
			_pc += 1;
			break;
		case -10:
			// mJX - PORT X
			set_port(_regs[low]);
			_pc += 1;
			break;
		case -9:
			// mIX - OUT X
			out(_regs[low]);
			_pc += 1;
			break;
		case -8:
			// mHX - DEC X
			decrement(_regs[low]);
			_pc += 1;
			break;
		case -7:
			// mGX t9 - SBB X, t9
			sub_with_borrow(_regs[low], _memory[_pc + 1]);
			_pc += 2;
			break;
		case -6:
			// mFX - POP X
			pop(_regs[low]);
			_pc += 1;
			break;
		case -5:
			// mEX t9 - SUB X, t9
			sub(_regs[low], _memory[_pc + 1]);
			_pc += 2;
			break;
		case -4:
			// mDX $Y - SAVE X, $Y
			save(_regs[mid], _memory[_pc + 1]);
			_pc += 2;
			break;
		case -3:
			// mCX t9 - SH X, t9
			trit_shift(_regs[low], _memory[_pc + 1]);
			_pc += 2;
			break;
		case -2:
			// mBX t9 - OR X, t9
			trit_or(_regs[low], _memory[_pc + 1]);
			_pc += 2;
			break;
		case 0:
			// m0X - ZERO X
			zero(_regs[low]);
			_pc += 1;
			break;
		case 1:
			// maX t9 - SET X, t9
			set(_regs[low], _memory[_pc + 1]);
			_pc += 2;
			break;
		case 2:
			// mbX t9 - AND X, t9
			trit_and(_regs[low], _memory[_pc + 1]);
			_pc += 2;
			break;
		case 3:
			// mcX t9 - CMP X, t9
			compare(_regs[low], _memory[_pc + 1]);
			_pc += 2;
			break;
		case 4:
			// mdY $X - LOAD $X, Y
			load(_memory[_pc + 1], _regs[low]);
			_pc += 2;
			break;
		case 5:
			// meX t9 - ADD X, t9
			add(_regs[low], _memory[_pc + 1]);
			_pc += 2;
			break;
		case 6:
			// mfX - PUSH X
			push(_regs[low]);
			_pc += 1;
			break;
		case 7:
			// mgX t9 - ADC X, t9
			add_with_carry(_regs[low], _memory[_pc + 1]);
			_pc += 2;
			break;
		case 8:
			// mhX - INC X
			increment(_regs[low]);
			_pc += 1;
			break;
		case 9:
			// miX - IN X
			in(_regs[low]);
			_pc += 1;
			break;
		case 10:
			// mjX - BANK X
			set_bank(_regs[low]);
			_pc += 1;
			break;
		case 11:
			// mkX t9 - STAR X, t9
			trit_star(_regs[low], _memory[_pc + 1]);
			_pc += 2;
			break;
		case 13:
			// mmX - PEEK X
			peek(_regs[low]);
			_pc += 1;
			break;
		}
		break;
	default:
		dump("Unrecognised instruction");
		halt();
		break;
	}	
}