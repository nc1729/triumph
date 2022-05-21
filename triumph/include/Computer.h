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
	Disk disk;
	// collate memory banks and initialise memory management
	Memory memory{ {&zero, &(screen.tryte_framebuffer), &(screen.tilemap), &(screen.work_RAM), &disk.buffer} };
	// init CPU
	CPU cpu{ memory };
	

public:
	Computer(std::string const& disk_path) :
    disk{1, disk_path},
	memory{{&zero, &screen.tryte_framebuffer, &screen.tilemap, &screen.work_RAM, &disk.buffer}},
	cpu{memory} {};
	void test_disk();
	void test(std::string const& tilemap_filename);
	void run_program(std::vector<Tryte> const& program);
};
