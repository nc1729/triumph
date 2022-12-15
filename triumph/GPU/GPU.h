#pragma once

#include "common/Tryte.h"
#include "Memory/MemoryBlock.h"
/*
GPU

The GPU contains all video memory - the two framebuffers, other work RAM and tilemap RAM.
These are exposed to the CPU through a public pointer to a MemoryBlock.

The GPU will pass the inactive framebuffer to the Screen every frame - this will then be
converted into bytes and then pushed into real video memory.
Effectively, we want to abstract away the actual hardware.
*/
class GPU
{
public:
    Tryte const BANK_START{ "MMM" };
	Tryte const BANK_END{ "DMM" };
	Tryte const PALETTE_START{ BANK_START + FRAMEBUF_SIZE };
private:
	MemoryBlock work_RAM{ "work_RAM", BANK_START, BANK_END};
	MemoryBlock tilemap_RAM{ "tilemap_RAM", BANK_START, BANK_END};
	MemoryBlock framebuffer{ "framebuffer", BANK_START, BANK_START + FRAMEBUF_SIZE};

public:
	GPU();

	int64_t static constexpr TILE_GRID_WIDTH = 36;
	int64_t static constexpr TILE_GRID_HEIGHT = 27;
	int64_t static constexpr FRAMEBUF_SIZE = TILE_GRID_HEIGHT * TILE_GRID_WIDTH;
	

	MemoryBlock* get_work_RAM() { return &work_RAM; }
	MemoryBlock* get_tilemap() { return &tilemap_RAM; }
	// switches between framebuffers - returns pointer to now inactive framebuffer
	MemoryBlock* get_framebuffer();

};