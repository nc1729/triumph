#include <vector>
#include <string>
#include <array>
#include <iostream>
#include <stdexcept>
#include <thread>

#include "Tryte.h"
#include "Computer.h"

void Computer::test_disk()
{
	Disk& disk = disks[0];
	disk.read_from_page(0);
	memory.bank() = 1;
	std::cout << memory[-9841] << memory[-9840] << memory[-9839] << '\n';
	for (int64_t i = Disk::DISK_BUFF_START; i < Disk::DISK_BUFF_END; i++)
	{
		memory[i] = 55;
	}
	disk.write_to_page(1);
}

void Computer::disk_copy()
{
	Disk& main_disk = disks[0];
	Disk& little_disk = disks[1];

	main_disk.read_from_page(1);
	memory.bank() = 2;
	little_disk.write_to_page(0);
}

void Computer::boot()
{
	// turn on computer
	is_on = true;

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

	// need to read tilemap from this disk into screen's tilemap memory
	size_t tilemap_first_page = static_cast<size_t>(Tryte::get_int(memory[Disk::DISK_TILEMAP_ADDR]) + 9841);
	memory.bank() = Screen::TILEMAP_BANK;

	for (size_t i = 0; i < 9; i++)
	{
		// read from boot_disk's file into its buffer
		boot_disk.read_from_page(tilemap_first_page + i);

		// and then copy buffer directly into the tilemap bank
		for (size_t j = 0; j < 729; j++)
		{
			memory[Memory::BANK_START + (729 * i) + j] = boot_disk.buffer[j];
		}
	}

	// next, read palettes into screen's work RAM
	memory.bank() = Screen::WORKRAM_BANK;
	boot_disk.read_from_page(0);
	for (size_t i = 0; i < 27; i++)
	{
		for (size_t j = 0; j < 3; j++)
		{
			memory[Memory::BANK_START + (3 * i) + j] = boot_disk.buffer[Disk::DISK_PALETTE_ADDR + 9841 + (3 * i) + j];
		}
	}

	// now swap back to the boot disk bank
	memory.bank() = boot_disk_number;

	// set stack pointer and program counter
	memory.sp() = Tryte("m00");
	memory.pc() = Tryte("M00"); // usually M00
	std::cout << memory.pc();

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
				// set status flag to busy
				memory[Disk::DISK_STATE_ADDR][Disk::STATUS_FLAG] = 0;
				// which page was requested?
				size_t page_number = static_cast<size_t>(Tryte::get_int(memory[Disk::DISK_PAGE_ADDR]) + 9841);
				// which disk is this?
				size_t disk_number = static_cast<size_t>(Tryte::get_int(memory.bank()) + 1);
				Disk& disk = disks[disk_number];
				// perform the read
				disk.read_from_page(page_number);
			}

			// check for a CPU write request
			if (memory[Disk::DISK_STATE_ADDR][Disk::WRITE_REQUEST_FLAG] == 1)
			{
				// the CPU has requested a write
				// set status flag to busy
				memory[Disk::DISK_STATE_ADDR][Disk::STATUS_FLAG] = 0;
				// which page was requested?
				size_t page_number = static_cast<size_t>(Tryte::get_int(memory[Disk::DISK_PAGE_ADDR]) + 9841);
				// which disk is this?
				size_t disk_number = static_cast<size_t>(Tryte::get_int(memory.bank()) + 1);
				Disk& disk = disks[disk_number];
				// perform the write
				disk.write_to_page(page_number);
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