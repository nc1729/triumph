#include "CPU.h"
#include "Tryte.h"

#include <iostream> // for show/tell

void CPU::load(Tryte const& x, Tryte& y)
{
	y = memory_[x];
}

void CPU::save(Tryte& x, Tryte const& y)
{
	memory_[y] = x;
}

void CPU::set_bank(Tryte const& x)
{
	bank_ = x;
}

void CPU::get_bank(Tryte& x)
{
	x = bank_;
}

void CPU::set(Tryte& x, Tryte const& y)
{
	x = y;
}

void CPU::swap(Tryte& x, Tryte& y)
{
	Tryte temp = x;
	x = y;
	y = temp;
}

void CPU::push(Tryte const& x)
{
	sp_ += -1;
	memory_[sp_] = x;
	if (sp_ < Tryte("mMM"))
	{
		// stack has overflowed
		STACK_FLAG = 1;
	}
}

void CPU::pop(Tryte& x)
{
	x = memory_[sp_];
	sp_ += 1;
	if (sp_ > Tryte("m00"))
	{
		// stack has underflowed
		STACK_FLAG = -1;
	}
}

void CPU::peek(Tryte& x)
{
	x = memory_[sp_];
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
	on_ = false;
}

void CPU::jump(Tryte const& x, Tryte const& y, Tryte const& z)
{
	if (COMPARE_FLAG < 0)
	{
		pc_ = x;
	}
	else if (COMPARE_FLAG == 0)
	{
		pc_ = y;
	}
	else
	{
		pc_ = z;
	}
}

void CPU::jump_and_store(Tryte const& x)
{
	// need to push the program counter to after this JPS $X instruction (two trytes)
	CPU::push(pc_ + 2);
	pc_ = x;
}

void CPU::pop_and_jump()
{
	CPU::pop(pc_);
}

void CPU::show(Tryte const& x)
{
	std::cout << x;
}

void CPU::tell(Tryte& x)
{
	std::cin >> x;
}