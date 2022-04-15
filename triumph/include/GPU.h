#pragma once

#include <SDL.h>
#include "Bank.h"

class GPU
{
private:
    // 6561 Trytes - 81x81 tiles + palette for each one
    Bank frame_buffer_;
    // at system boot, should fill this bank with 729 tiles (9 Trytes each)
    Bank tilemap_;
    // general RAM for GPU's use. Hold config variables here (video mode), sprite data, floating point?
    Bank workRAM_;

    SDL_Window* window_ = nullptr;
    SDL_Renderer* renderer_ = nullptr;
    SDL_Texture* texture_ = nullptr;
    

public:
    GPU() = delete;
    GPU(int64_t const frame_buffer_bank_number,
        int64_t const tilemap_bank_number,
        int64_t const workRAM_bank_number);
};