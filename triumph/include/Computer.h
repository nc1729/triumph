#pragma once

#include <vector>
#include <iostream>
#include <string>

#include "Bank.h"
#include "Memory.h"
#include "CPU.h"
#include "Disk.h"
#include "Screen.h"

class Computer
{
private:
	// init Screen - contains SDL calls
	Screen screen;
	// general purpose memory
	Bank zero{ 0 };
	// disk
	std::vector<Disk>& disks;
	// collate memory banks and initialise memory management
	Memory memory{ {&zero, &(screen.tryte_framebuffer), &(screen.tilemap), &(screen.work_RAM)}, disks };
	// init CPU
	CPU cpu{ memory };
	

public:
	Computer(std::vector<Disk>& disks) :
    disks{disks},
	memory{{&zero, &screen.tryte_framebuffer, &screen.tilemap, &screen.work_RAM}, disks},
	cpu{memory} {};

	void test_disk();
	void test(std::string const& tilemap_filename);
	void run_program(std::vector<Tryte> const& program);
};
