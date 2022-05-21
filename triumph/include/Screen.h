#pragma once

#include <SDL.h>
#include <string>
#include <vector>
#include <cstdint>
#include <array>

#include "Bank.h"

class Screen
{
private:
	// SDL2 internals
	SDL_Window* window = nullptr;
	SDL_Surface* surface = nullptr; // software rendering
	SDL_Renderer* renderer = nullptr;
	SDL_Texture* screen_texture = nullptr;
	std::vector<uint32_t> byte_framebuffer;

	// screen size
	uint64_t const TILE_GRID_WIDTH = 54;
	uint64_t const TILE_GRID_HEIGHT = 54;
	uint64_t const PIXEL_WIDTH = TILE_GRID_WIDTH * 9;
	uint64_t const PIXEL_HEIGHT = TILE_GRID_HEIGHT * 9;
	
	// palettes are stored at start of work_RAM - 27 palettes, 81 Trytes
	// 3 colours per palette, each colour is a tryte (high - R, mid - G, low - B)
	uint64_t const PALETTE_START = 0; // $MMM
	std::vector<uint32_t> palettes{ 81, 0 };
	std::array<uint32_t, 27> static colour_values;

	const char* window_title = "SDL Test";

	void write_tryte_fb_to_byte_fb();

	void write_tile_to_framebuffer(size_t const grid_index_x, size_t const grid_index_y, size_t const palette_index, size_t const tile_addr);
	// using the 81 trytes stored in palette memory, construct the byte palettes array
	void regen_palettes();
	uint32_t static tryte_to_colour(Tryte const& colour_tryte);
	
public:
	Screen();
	~Screen();
	void test();
	void read_tilemap(std::string const& filename);
	void show_tilemap();
	void hello_world();

	// VRAM (accessible as banks -1, -2, -3 by CPU)
	Bank tryte_framebuffer{ -1 };
	Bank tilemap{ -2 };
	Bank work_RAM{ -3 };
	
};
void show_window();