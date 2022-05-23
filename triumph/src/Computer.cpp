#include <vector>
#include <string>
#include <array>
#include <iostream>
#include <stdexcept>
#include <thread>

#include "Tryte.h"
#include "Computer.h"

void Computer::turn_on()
{
	is_on = true;
	boot();
}
void Computer::boot()
{
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

	// start disk manager in new thread
	std::thread disk_thread{ &Computer::disk_manager, this };

	// start CPU in new thread
	std::thread cpu_thread{ &CPU::run, &(this->cpu) };

	// turn screen on in main thread
	screen.is_on = true;
	screen.run();

	// when screen is switched off, stop the CPU
	cpu.turn_off();
	cpu_thread.join();
	this->is_on = false;
	disk_thread.join();

}

void Computer::disk_manager()
{
	while (this->is_on)
	{
		// check if a disk is being accessed
		if (memory.bank() > Tryte(0))
		{
			// check for a CPU read request
			if (memory[Disk::DISK_STATE_ADDR][Disk::READ_REQUEST_FLAG] == 1)
			{
				// the CPU has requested a read
				
				// set disk to busy
				memory[Disk::DISK_STATE_ADDR][Disk::STATUS_FLAG] = 0;

				// which page was requested?
				size_t page_number = static_cast<size_t>(Tryte::get_int(memory[Disk::CACHE_PAGE_ADDR]) + 9841);
				// which disk is this?
				size_t disk_number = static_cast<size_t>(Tryte::get_int(memory.bank()) + 1);
				Disk& disk = disks[disk_number];

				// perform the read
				disk.read_from_page(page_number);

				// set disk to free
				memory[Disk::DISK_STATE_ADDR][Disk::STATUS_FLAG] = 1;

				// reset read request flag
				memory[Disk::DISK_STATE_ADDR][Disk::READ_REQUEST_FLAG] = 0;
			}

			// check for a CPU write request
			if (memory[Disk::DISK_STATE_ADDR][Disk::WRITE_REQUEST_FLAG] == 1)
			{
				// the CPU has requested a write
				 
				// set disk to busy
				memory[Disk::DISK_STATE_ADDR][Disk::STATUS_FLAG] = 0;

				// which page was requested?
				size_t page_number = static_cast<size_t>(Tryte::get_int(memory[Disk::CACHE_PAGE_ADDR]) + 9841);
				// which disk is this?
				size_t disk_number = static_cast<size_t>(Tryte::get_int(memory.bank()) + 1);
				Disk& disk = disks[disk_number];
				// perform the write
				disk.write_to_page(page_number);

				// set disk to free
				memory[Disk::DISK_STATE_ADDR][Disk::STATUS_FLAG] = 1;

				// reset write request flag
				memory[Disk::DISK_STATE_ADDR][Disk::WRITE_REQUEST_FLAG] = 0;

			}
		}
	}
}
void Computer::test(std::string const& tilemap_filename)
{
	screen.read_tilemap(tilemap_filename);
	screen.hello_world();

}
void Computer::run_program(std::vector<Tryte> const& program)
{
	memory.load_program(Tryte::get_int(Tryte("eMM")), program);
	
	cpu.turn_on();

	cpu.run();
}