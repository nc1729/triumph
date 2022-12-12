#include "common/Tryte.h"
#include "Memory/MemoryBlock.h"
#include "GPU/GPU.h"

GPU::GPU()
{
	// second framebuffer is accessible only by changing bank of work_RAM
	work_RAM.add_bank(&framebuffer, 1);
}

MemoryBlock* GPU::get_framebuffer()
{
	// switch bank from 0 to 1 or vice-versa
	if (work_RAM.bank() == 0)
	{
		// switch to other framebuffer
		work_RAM.bank() == 1;
		// pass work RAM framebuffer to Screen
		return &work_RAM;
	}
	else
	{
		// switch to work RAM framebuffer
		work_RAM.bank() == 0;
		// pass second framebuffer to Screen
		return &framebuffer;
	}
}