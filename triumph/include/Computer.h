#pragma once

#include <vector>
#include <iostream>
#include <string>

#include "Bank.h"
#include "Memory.h"
#include "CPU.h"
#include "Screen.h"

class Computer
{
private:
	// init Screen - contains SDL calls
	Screen screen;
	// general purpose memory
	Bank zero{ 0 };
	// collate memory banks and initialise memory management
	Memory memory{ {&zero, &(screen.tryte_framebuffer), &(screen.tilemap), &(screen.work_RAM)} };
	// init CPU
	CPU cpu{ memory };
	

public:
	void test(std::string const& tilemap_filename);
	void run_program(std::vector<Tryte> const& program);
};
