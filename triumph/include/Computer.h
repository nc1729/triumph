#pragma once

#include <vector>
#include <iostream>

#include "Bank.h"
#include "Memory.h"
#include "CPU.h"
#include "PortManager.h"
#include "Keyboard.h"
#include "ConsoleOut.h"

class Computer
{
private:
	Bank bank0{ 0 };
	Bank bank1{ 1 };
	std::vector<Bank*> banks = { &bank0, &bank1 };
        Memory memory;
	PortManager ports;
	CPU cpu{ memory, ports };

public:
	Computer() :
	    memory{banks}
	{};

	void run_program(std::vector<Tryte> const& program);
};
