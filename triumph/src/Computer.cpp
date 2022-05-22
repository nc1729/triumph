#include <vector>
#include <string>
#include <iostream>

#include "Tryte.h"
#include "Computer.h"

void Computer::test_disk()
{
	std::cout << "Opening disk 0\n";
	Disk& disk = disks[0];
	disk.read_from_page(0);
	std::cout << "Read from page 0\n";
	memory.bank() = 1;
	std::cout << "Changed to memory bank 1\n";
	std::cout << memory[-9841] << memory[-9840] << memory[-9839] << '\n';
	for (int64_t i = Disk::DISK_BUFF_START; i < Disk::DISK_BUFF_END; i++)
	{
		memory[i] = 55;
	}
	disk.write_to_page(1);
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