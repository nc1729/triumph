#pragma once

#include <SDL.h>
#include <string>

#include "Bank.h"

class Screen
{
private:
	// video RAM
	Bank& tryte_frame_buffer_;
	Bank& tilemap_;
	Bank& work_RAM_;

	// SDL2 internals
	SDL_Window* window = nullptr;
	SDL_Surface* surface = nullptr; // software rendering

	// screen size
	uint64_t const TILE_GRID_WIDTH_ = 54;
	uint64_t const TILE_GRID_HEIGHT_ = 54;

	const char* window_title_ = "SDL Test";

public:
	Screen() = delete;
	Screen(Bank& tryte_frame_buffer, Bank& tilemap, Bank& work_RAM);
	~Screen();
	void test();
};
void show_window();