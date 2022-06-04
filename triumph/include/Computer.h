#pragma once

#include <vector>
#include <iostream>
#include <string>

#include "Bank.h"
#include "Console.h"
#include "Memory.h"
#include "CPU.h"
#include "Disk.h"
#include "Screen.h"

class Computer
{
private:
	// init Screen - contains SDL calls
	Screen screen;
	// Console interface
	Console console;
	// general purpose memory
	Bank zero{ 0 };
	// disk
	std::vector<Disk>& disks;
	// collate memory banks and initialise memory management
	Memory memory{ {&zero, &(screen.tryte_framebuffer), &(screen.tilemap), &(screen.work_RAM), &(console.buffer)}, disks};
	// init CPU
	CPU cpu{ memory };
	

public:
	Computer(std::vector<Disk>& disks) :
	console{std::cout, std::cin},
    disks{disks},
	memory{{&zero, &screen.tryte_framebuffer, &screen.tilemap, &screen.work_RAM, &console.buffer}, disks},
	cpu{memory} {};

	bool is_on = false;
	void turn_on();
	void boot();
	void disk_manager();
	void console_manager();
};
