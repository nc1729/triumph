#include <vector>
#include <string>

#include "Tryte.h"
#include "Computer.h"

void Computer::test(std::string const& tilemap_filename)
{
	screen.read_tilemap(tilemap_filename);
	screen.show_tilemap();

}
void Computer::run_program(std::vector<Tryte> const& program)
{
	memory.load_program(Tryte::get_int(Tryte("eMM")), program);
	
	cpu.turn_on();

	cpu.run();
}