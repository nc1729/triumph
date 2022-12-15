#include "common/constants.h"
#include "common/Tryte.h"
#include "Memory/MemoryBlock.h"

MemoryBlock::MemoryBlock(Tryte const& start, Tryte const& end) :
	start_{start}, end_{end}
{
	// initialise local memory to zero
	local_[start_] = constants::zero;
	for (Tryte index = start_ + 1; index != end_; ++index)
	{
		local_[index] = constants::zero;
	}
}

Tryte& MemoryBlock::operator[](Tryte const& addr)
{
	if (current_ == constants::zero)
	{
		// bank 0 always resolves to local memory
		return local_.at(addr);
	}
	else
	{
		MemoryBlock* current_bank = banks_.at(current_);
		if (addr < current_bank->end() && addr >= current_bank->begin())
		{
			return (*current_bank)[addr];
		}
		else
		{
			return local_.at(addr);
		}
	}
}

Tryte const& MemoryBlock::operator[](Tryte const& addr) const
{
	if (current_ == constants::zero)
	{
		// bank 0 always resolves to this block
		return local_.at(addr);
	}
	else
	{
		return (*banks_.at(current_))[addr];
	}
}

void MemoryBlock::add_bank(MemoryBlock* bank_ptr, Tryte const& index)
{
	banks_[index] = bank_ptr;
}

void MemoryBlock::remove_bank(Tryte const& index)
{
	banks_.erase(index);
}
