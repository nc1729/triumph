#include "common/constants.h"
#include "common/Tryte.h"
#include "Memory/MemoryBlock.h"
#include <iostream>

MemoryBlock::MemoryBlock(std::string const& name, Tryte const& start, Tryte const& end) :
	name{name}, start_{start}, end_{end}
{
	// initialise local memory to zero
	local_[start_] = constants::zero;
	for (Tryte index = start_ + 1; index != end_; ++index)
	{
		local_[index] = constants::zero;
	}
	std::cout << "Constructed MemoryBlock " << name << " with size " << this->size() << '\n';
}

Tryte& MemoryBlock::operator[](Tryte const& addr)
{
	if (current_ == constants::zero)
	{
		// bank 0 always resolves to local memory
		try
		{
			return local_.at(addr);
		}
		catch(const std::out_of_range& e)
		{
			std::cout << e.what() << '\n';
			std::cout << "Tried to access address $" << addr << " in bank " << name << '\n';
			abort();
		}
	}
	else
	{
		MemoryBlock* current_bank = nullptr;
		try
		{
			current_bank = banks_.at(current_);
		}
		catch(const std::out_of_range& e)
		{
			std::cout << e.what() << '\n';
			std::cout << "Bank " << current_.get_int() << " in bank " << name << " doesn't exist!";
			abort();
		}
		
		if (addr < current_bank->end() && addr >= current_bank->begin())
		{
			return (*current_bank)[addr];
		}
		else
		{
			try
			{
				return local_.at(addr);
			}
			catch(const std::out_of_range& e)
			{
				std::cout << e.what() << '\n';
				std::cout << "Tried to access address $" << addr << " in bank " << name << '\n';
				abort();
			}
		}
	}
}

Tryte const& MemoryBlock::operator[](Tryte const& addr) const
{
	if (current_ == constants::zero)
	{
		// bank 0 always resolves to local memory
		try
		{
			return local_.at(addr);
		}
		catch(const std::out_of_range& e)
		{
			std::cout << "MemoryBlock::operator[] tried to access invalid address in local_\n";
			std::cout << "Tried to access address $" << addr << " in bank " << name << '\n';
			abort();
		}
	}
	else
	{
		MemoryBlock* current_bank = nullptr;
		try
		{
			current_bank = banks_.at(current_);
		}
		catch(const std::out_of_range& e)
		{
			std::cout << "MemoryBlock::operator[] tried to access bad bank\n";
			std::cout << "Bank " << current_.get_int() << " in bank " << name << " doesn't exist!";
			abort();
		}
		
		if (addr < current_bank->end() && addr >= current_bank->begin())
		{
			return (*current_bank)[addr];
		}
		else
		{
			try
			{
				return local_.at(addr);
			}
			catch(const std::out_of_range& e)
			{
				std::cout << "MemoryBlock::operator[] tried to access invalid address in local_\n with current = " << current_ << '\n';
			    std::cout << "Tried to access address $" << addr << " in bank " << name << '\n';
			    abort();
			}
		}
	}
}

void MemoryBlock::add_bank(MemoryBlock* bank_ptr, Tryte const& index)
{
	banks_[index] = bank_ptr;
	std::cout << "Added " << bank_ptr->name << " bank to " << this->name << " at index " << index.get_int() << '\n';
}

void MemoryBlock::remove_bank(Tryte const& index)
{
	std::cout << "Removed " << banks_[index]->name << " bank from " << this->name << '\n';
	banks_.erase(index);
	
}

int64_t MemoryBlock::size() const
{
	Tryte diff = end_ - start_;
	int64_t diff_int = diff.get_int();
	if (diff_int <= 0)
	{
		diff_int += 19683;
	}
	return diff_int;
}