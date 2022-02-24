#include <vector>
#include <string>
#include <array>
#include <stdexcept>

#include "Memory.h"
#include "Bank.h"
#include "Tryte.h"

Memory::Memory(size_t const number_of_banks, std::vector<Bank*> const& bank_ptrs) : banks_(bank_ptrs)
{
	// initialise local memory
	local_.fill(0);

	// write boot code to memory
	init();

}

void Memory::init()
{
	this->sp() = Tryte("m00"); // move stack pointer to $m00
	this->pc() = Tryte("m00"); // move program counter to $m00
	std::vector<std::string> const boot_code =
	{
		"m0a", "m0b", // $m00-m0h:   ZERO all registers
		"m0c", "m0d",
		"m0e", "m0f",
		"m0g", "m0h",
		"m0i",
		"mja", "mJa", // $m0i, $m0j: PORT A; BANK A - set PORT to 0, BANK to 0
		"mai", "eMM", // $m0k, $m0l: SET I, #eMM - set I to start of PROGRAM memory
		/*
		Main loop: read from standard input until end-of-file, then break out of loop
		*/
		              //             !start
        "mib",	      // $m0m:       IN B - read from PORT 0
        "mGf", "0c0", // $maM, $maL: STAR F, #0c0 - set compare flag to value of PORT flag
        "jjA", "maF", // $maK, $maJ: JPN end - if PORT flag was negative, break out of read loop
        "Dbi",        // $maI:       SAVE B, [I] - write read-in Tryte to PROGRAM memory
        "mhi",   	  // $maH:       INC I - increment memory pointer
        "jj0", "m0l"  // $maG, $maF: JP start - loop until PORT flag is negative
			          //             !end
        /*
        end main loop - program from stdin is now in PROGRAM region of memory
        !! Caution - program must not be longer than 5,832 Trytes, or program will grow into stack!
        !! An error will occur if this loop tries to overwrite itself (boot memory is read-only)
        */
        "m0b", "m0f", // $maE:       ZERO B; ZERO F - clear all registers used
        "m0i",        // $maD:       ZERO I
        "jj0", "eMM"  // $maC, $maB: JP $eMM - jump to start of PROGRAM memory and begin executing program!
	};

	int64_t boot_code_size = boot_code.size();
	int64_t index = 12757; // _local array - maps to address $m00 - (2 * 6561 - 365) -> 9841 - 365
	for (int64_t i = 0; i < boot_code_size; i++)
	{
		local_[index + i] = boot_code[i];
		index++;
	}
	while (index < 2 * 6561 - 4) // stop before overwriting PORT, BANK etc!  
	{
		// fill 'spare' memory with #MMM = -9841 - HALT instructions
		local_[index] = -9841;
		index++;
	}
}

Tryte& Memory::operator[](int64_t const addr)
{
	if (addr < Memory::BANK_END)
	{
		// accessing memory bank
		return banks_(Tryte::get_int((*this).bank()), addr);
	}
	else if (addr > Memory::RESERVED_START)
	{
		// attempted to access reserved memory
		throw std::runtime_error("Attempted to access reserved memory location: " + Tryte::get_str(Tryte(addr)));
	}
	else
	{
		return local_[addr - Memory::BANK_END];
	}
}

Tryte const& Memory::operator[](int64_t const addr) const
{
	if (addr < Memory::BANK_END)
	{
		// accessing memory bank
		return banks_(Tryte::get_int((*this)[Memory::BANK]), addr);
	}
	else if (addr > Memory::RESERVED_START)
	{
		// attempted to access reserved memory
		throw std::runtime_error("Attempted to access reserved memory location: " + Tryte::get_str(Tryte(addr)));
	}
	else
	{
		return local_[addr - Memory::BANK_END];
	}
}

Tryte& Memory::port()
{
	return local_[Memory::PORT + 9841 - 6561];
}

Tryte& Memory::bank()
{
	return local_[Memory::BANK + 9841 - 6561];
}

Tryte& Memory::sp()
{
	return local_[Memory::SP + 9841 - 6561];
}

Tryte& Memory::pc()
{
	return local_[Memory::PC + 9841 - 6561];
}