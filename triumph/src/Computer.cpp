#include <vector>
#include <string>
#include <array>
#include <iostream>
#include <stdexcept>
#include <thread>

#include "Tryte.h"
#include "Bank.h"
#include "Console.h"
#include "Computer.h"

void Computer::turn_on()
{
	is_on = true;
	boot();
}
void Computer::boot()
{
	// can't boot if the computer is switched off...
	if (!is_on)
	{
		return;
	}

	// find a bootable disk
	size_t boot_disk_number = 0;
	for (auto& disk : disks)
	{
		if (disk.is_bootable)
		{
			// boot from first bootable disk found
			boot_disk_number = disk.number;
			break;
		}
	}

	// if no bootable disk found, throw error
	if (!boot_disk_number)
	{
		throw std::runtime_error("No bootable disk found!");
	}

	// disks start counting from 0, disk numbers (as bank numbers) start from 1
	Disk& boot_disk = disks[boot_disk_number - 1];

	// now set memory bank to boot disk
	memory.bank() = boot_disk_number;

	// set stack pointer and program counter
	memory.sp() = Tryte("m00");
	memory.pc() = boot_disk.buffer[BootDisk::BOOTCODE_PTR_ADDR]; // usually M00

	// start IO in new thread
	std::thread IO_thread{ &Computer::IO_manager, this };

	// start CPU in new thread
	std::thread cpu_thread{ &CPU::run, &(this->cpu) };

	// turn screen on in main thread
	screen.is_on = true;
	screen.run();

	// when screen is switched off, stop the CPU
	cpu.turn_off();
	cpu_thread.join();
	this->is_on = false;
	IO_thread.join();

}

void Computer::disk_manager()
{
	// check if a disk is being accessed
	if (memory.bank() > Tryte(0))
	{
		// check for a CPU read request
		if (memory[Disk::DISK_STATE_ADDR][Disk::READ_REQUEST_FLAG] == 1)
		{
			// the CPU has requested a read

			// which page was requested?
			int64_t page_number = memory[Disk::CACHE_PAGE_ADDR];
			// which disk is this?
			size_t disk_number = static_cast<size_t>(memory.bank() - 1);
			Disk& disk = disks[disk_number];

			// perform the read
			disk.read_from_page(page_number);

			// reset read request flag
			memory[Disk::DISK_STATE_ADDR][Disk::READ_REQUEST_FLAG] = 0;
		}

		// check for a CPU write request
		if (memory[Disk::DISK_STATE_ADDR][Disk::WRITE_REQUEST_FLAG] == 1)
		{
			// the CPU has requested a write

			// which page was requested?
			size_t page_number = static_cast<size_t>(memory[Disk::CACHE_PAGE_ADDR] + 9841);
			// which disk is this?
			size_t disk_number = static_cast<size_t>(memory.bank() + 1);
			Disk& disk = disks[disk_number];
			// perform the write
			disk.write_to_page(page_number);


			// reset write request flag
			memory[Disk::DISK_STATE_ADDR][Disk::WRITE_REQUEST_FLAG] = 0;

		}
	}
}

void Computer::console_manager()
{
	// check if console is being accessed
	if (memory.bank() == Tryte(Console::CONSOLE_BANK))
	{
		// check for an out request
		if (memory[Console::CONSOLE_STATE_ADDR][Console::OUT_REQUEST_FLAG] == 1)
		{
			// the CPU has requested to flush the buffer, so flush it
			console.flush_to_out();
			// and reset the console out flag
			memory[Console::CONSOLE_STATE_ADDR][Console::OUT_REQUEST_FLAG] = 0;
		}

		// check for an in request
		if (memory[Console::CONSOLE_STATE_ADDR][Console::IN_REQUEST_FLAG] == 1)
		{
			// the CPU has requested input from stdin
			console.read_from_in();
			// reset console input flag
			memory[Console::CONSOLE_STATE_ADDR][Console::IN_REQUEST_FLAG] = 0;
		}
	}
	console.set_display_mode();
}

void Computer::IO_manager()
{
	while (this->is_on)
	{
		console_manager();
		disk_manager();
	}
}