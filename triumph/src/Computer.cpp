#include <vector>

#include "Tryte.h"
#include "Computer.h"

void Computer::run_program(std::vector<Tryte> const& program)
{
	memory.load_program(Tryte::get_int(Tryte("eMM")), program);
	
	cpu.turn_on();

	cpu.run();
}