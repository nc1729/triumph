#pragma once

#include <vector>
#include <iostream>
#include <string>

#include "TriumphCLOptions.h"
#include "Bank.h"
#include "Console.h"
#include "Memory.h"
#include "CPU.h"
#include "Disk.h"
#include "Screen.h"

class Computer
{
private:
	// options from command line
	TriumphCLOptions options;
	// init Screen - contains SDL calls
	Screen screen;
	// Console interface
	Console console;
	// general purpose memory
	Bank zero{ 0 };
	// disk
	std::vector<Disk>& disks;
	// collate memory banks and initialise memory management
	Memory memory{ {&zero, &(console.buffer)}, disks};
	// init CPU
	CPU cpu{ memory };
	
	void disk_manager();
	void console_manager();
public:
	Computer(std::vector<Disk>& disks, TriumphCLOptions const& options) :
	options{options},
	console{std::cout, std::cin},
    disks{disks},
	memory{{&zero, &screen.tryte_framebuffer, &screen.tilemap, &screen.work_RAM, &console.buffer}, disks},
	cpu{memory} {};

	bool is_on = false;
	void turn_on();
	void boot();
	// disk and console run in the same thread to prevent race conditions
	void IO_manager();
};
