#include "CPU.h"
#include "Tryte.h"

void CPU::load(Tryte const& x, Tryte& y)
{
	y = _memory[x];
}

void CPU::save(Tryte& x, Tryte const& y)
{
	_memory[y] = x;
}

void CPU::set_bank(Tryte const& x)
{
	_bank = x;
}

void CPU::set(Tryte& x, Tryte const& y)
{
	x = y;
}

void CPU::push(Tryte const& x)
{
	_sp += -1;
	_memory[_sp] = x;
	if (_sp < Tryte("mMM"))
	{
		// stack has overflowed
		STACK_FLAG = 1;
	}
}

void CPU::pop(Tryte& x)
{
	x = _memory[_sp];
	_sp += 1;
	if (_sp > Tryte("m00"))
	{
		// stack has underflowed
		STACK_FLAG = -1;
	}
}

void CPU::peek(Tryte& x)
{
	x = _memory[_sp];
}

void CPU::in(Tryte& x)
{
	x << _devices[_port];
}

void CPU::out(Tryte const& x)
{
	x >> _devices[_port];
}

void CPU::set_port(Tryte const& x)
{
	_port = x;
}

void CPU::add(Tryte& x, Tryte const& y)
{
	CARRY_FLAG = 0;
	Tryte::add_with_carry(x, y, CARRY_FLAG);
	SIGN_FLAG = Tryte::sign(x);
}

void CPU::add_with_carry(Tryte& x, Tryte const& y)
{
	Tryte::add_with_carry(x, y, CARRY_FLAG);
	SIGN_FLAG = Tryte::sign(x);
}

void CPU::sub(Tryte& x, Tryte const& y)
{
	CARRY_FLAG = 0;
	Tryte::subtract_with_borrow(x, y, CARRY_FLAG);
	SIGN_FLAG = Tryte::sign(x);
}

void CPU::sub_with_borrow(Tryte& x, Tryte const& y)
{
	Tryte::subtract_with_borrow(x, y, CARRY_FLAG);
	SIGN_FLAG = Tryte::sign(x);
}

void CPU::trit_shift(Tryte& x, Tryte const& y)
{
	Tryte::shift(x, y);
	SIGN_FLAG = Tryte::sign(x);
}

void CPU::increment(Tryte& x)
{
	add(x, 1);
	SIGN_FLAG = Tryte::sign(x);
}

void CPU::decrement(Tryte& x)
{
	add(x, -1);
	SIGN_FLAG = Tryte::sign(x);
}

void CPU::flip(Tryte& x)
{
	Tryte::flip(x);
}

void CPU::zero(Tryte& x)
{
	x = 0;
}

void CPU::compare(Tryte const& x, Tryte const& y)
{
	if (x < y)
	{
		COMPARE_FLAG = -1;
	}
	else if (x > y)
	{
		COMPARE_FLAG = 1;
	}
	else
	{
		COMPARE_FLAG = 0;
	}
}

void CPU::trit_and(Tryte& x, Tryte const& y)
{
	x &= y;
	SIGN_FLAG = Tryte::sign(x);
}

void CPU::trit_or(Tryte& x, Tryte const& y)
{
	x |= y;
	SIGN_FLAG = Tryte::sign(x);
}

void CPU::trit_star(Tryte& x, Tryte const& y)
{
	Tryte::star(x, y);
	SIGN_FLAG = Tryte::sign(x);
}

void CPU::nop()
{
	// do nothing
}

void CPU::halt()
{
	// stop execution, will need to be turned back on
	_on = false;
}

void CPU::jump_if_zero(Tryte const& x)
{
	if (COMPARE_FLAG == 0)
	{
		_pc = x;
	}
}

void CPU::jump_if_not_zero(Tryte const& x)
{
	if (COMPARE_FLAG != 0)
	{
		_pc = x;
	}
}

void CPU::jump_if_pos(Tryte const& x)
{
	if (COMPARE_FLAG > 0)
	{
		_pc = x;
	}
}

void CPU::jump_if_neg(Tryte const& x)
{
	if (COMPARE_FLAG < 0)
	{
		_pc = x;
	}
}

void CPU::jump(Tryte const& x)
{
	_pc = x;
}

void CPU::jump_and_store(Tryte const& x)
{
	CPU::push(_pc);
	_pc = x;
}

void CPU::pop_and_jump()
{
	CPU::pop(_pc);
}

void CPU::ternary_jump(Tryte const& x, Tryte const& y, Tryte const& z)
{
	if (COMPARE_FLAG < 0)
	{
		_pc = x;
	}
	else if (COMPARE_FLAG == 0)
	{
		_pc = y;
	}
	else
	{
		_pc = z;
	}
}