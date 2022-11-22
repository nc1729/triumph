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
	SDL_Renderer* renderer = nullptr;
	SDL_Texture* screen_texture = nullptr;
	std::vector<uint32_t> byte_framebuffer;

	// screen size
	size_t const PIXELS_PER_TRIT = 2;
	size_t const PIXELS_PER_TILE = 9 * PIXELS_PER_TRIT;
	int const TILE_GRID_WIDTH = 36;
	int const TILE_GRID_HEIGHT = 27;
	int const PIXEL_WIDTH = TILE_GRID_WIDTH * PIXELS_PER_TILE;
	int const PIXEL_HEIGHT = TILE_GRID_HEIGHT * PIXELS_PER_TILE;
	
	// palettes are stored at start of work_RAM - 27 palettes, 81 Trytes
	// 3 colours per palette, each colour is a tryte (high - R, mid - G, low - B)
	uint64_t const PALETTE_START = 0; // $MMM
	std::vector<uint32_t> palettes{ 81, 0 };
	std::array<uint32_t, 27> static const colour_values;

	const char* window_title = "TRIUMPH";

	// convert the tryte framebuffer to byte framebuffer that SDL2 can render to screen
	void write_tryte_fb_to_byte_fb();

	// write a given tile in the tilemap buffer to a given x, y spot in the tryte framebuffer
	void write_tile_to_framebuffer(size_t const grid_index_x, size_t const grid_index_y, size_t const palette_index, size_t const tile_addr);
	// using the 81 trytes stored in palette memory, construct the byte palettes array
	void regen_palettes();
	// convert a RGB Tryte (high - red, mid - green, low - blue) into a color SDL2 can understand
	uint32_t static tryte_to_colour(Tryte const& colour_tryte);

	// debugging functions for dumping memory contents to console
	void dump_bank(Bank const& bank);

	// address in work_RAM bank for sharing status with CPU
	int64_t static constexpr STATUS = 6560;
	
public:
	void read_tilemap(std::string const& filename);
	void show_tilemap();

	// main loop
	void run();

	// on/off functions - init/destroy SDL when screen is turned on/off
	void turn_on();
	void turn_off();

	void draw_to_screen();
	int64_t static constexpr FPS = 60;

	int64_t static constexpr FRAMEBUFFER_BANK = -101;
	int64_t static constexpr TILEMAP_BANK = -102;
	int64_t static constexpr WORKRAM_BANK = -103;
	// location of screen status flag in work RAM
	// + : screen is ready
	// 0 : screen is busy
	// - : screen is off or has an error
	int64_t static constexpr STATUS_ADDR = STATUS - 9841;
	

	// VRAM (accessible as banks -1, -2, -3 by CPU)
	Bank tryte_framebuffer{ FRAMEBUFFER_BANK };
	Bank tilemap{ TILEMAP_BANK };
	Bank work_RAM{ WORKRAM_BANK };

	bool is_on;
	
};
