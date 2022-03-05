#pragma once

#include <vector>

#include "CPU.h"
#include "Memory.h"
#include "Bank.h"

class Computer
{
private:
	Bank bank0{ 0 };
	Bank bank1{ 1 };
	std::vector<Bank*> banks = { &bank0, &bank1 };
	Memory memory{ banks.size(), banks };
	PortManager ports;
};