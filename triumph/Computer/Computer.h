#pragma once

#include <vector>
#include <iostream>
#include <string>

#include "Computer/TriumphCLOptions.h"
#include "CPU/CPU.h"
#include "IO/Disk.h"
#include "IO/Screen.h"
#include "Memory/MemoryBlock.h"


class Computer
{
private:
	// options from command line
	TriumphCLOptions options;
	// init Screen - contains SDL calls
	Screen screen;
	GPU gpu;
	std::vector<Disk>& disks;
	// main memory
	MemoryBlock memory;
	// init CPU
	CPU cpu{ memory };
	
	void disk_manager();
	void console_manager();
public:
	Computer(std::vector<Disk>& disks, TriumphCLOptions const& options);
	/* :
	options{options},
	console{std::cout, std::cin},
    disks{disks},
	memory{{&zero, &screen.tryte_framebuffer, &screen.tilemap, &screen.work_RAM, &console.buffer}, disks},
	cpu{memory} {};
	*/

	bool is_on = false;
	void turn_on();
	void boot();
	void BIOS();
	// disk and console run in the same thread to prevent race conditions
	void IO_manager();
};
