#pragma once

#include "common/Tryte.h"
#include "common/constants.h"
#include "Memory/MemoryBlock.h"

class BIOS
{
private:
	Tryte const SIZE = 729;
	MemoryBlock chip{ constants::CPU_START, constants::CPU_START + SIZE };

public:
	BIOS();
	MemoryBlock* get_bank() { return &chip; }
};
