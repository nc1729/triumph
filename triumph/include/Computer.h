#pragma once

#include <vector>
#include <iostream>
#include <string>

#include "Bank.h"
#include "Memory.h"
#include "CPU.h"
#include "PortManager.h"
#include "Screen.h"

class Computer
{
private:
	// general purpose memory
	Bank zero{ 0 };
	// VRAM banks
	Bank framebuffer{ 1 };
	Bank tilemap{ 2 };
	Bank work_RAM{ 3 };
	// collate memory banks and initialise memory management
	Memory memory{ {zero, framebuffer, tilemap, work_RAM} };
	// init I/O ports (might change this for memory mapped I/O - more banks!)
	PortManager ports;
	// init CPU
	CPU cpu{ memory, ports };
	// init Screen - contains SDL calls
	Screen screen{ framebuffer, tilemap, work_RAM };

public:
	void test(std::string const& tilemap_filename);
	void run_program(std::vector<Tryte> const& program);
};
