#include <SDL.h>
#include <iostream>
#include <string>
#include <fstream>
#include <array>
#include <cstdint>
#include <cmath>

#include "common/Tryte.h"
#include "GPU/GPU.h"
#include "IO/Screen.h"
#include "Memory/MemoryBlock.h"

std::array<uint32_t, 27> const Screen::colour_values = 
{ 0, 10, 20, 29, 39, 49, 59, 69, 78, 88, 98, 108, 118, 128, 137, 147, 157, 167, 177, 186,
    196, 206, 216, 226, 235, 245, 255 };

void Screen::turn_on()
{
    // initialise SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "SDL could not initialise! SDL Error: " << SDL_GetError() << '\n';
        std::string SDL_err_code = SDL_GetError();
        std::string err_string = "SDL could not initialise! SDL Error: ";
        throw std::runtime_error(err_string + SDL_err_code);
    }
    else
    {
        // create window
        window = SDL_CreateWindow(window_title, 
            SDL_WINDOWPOS_UNDEFINED, 
            SDL_WINDOWPOS_UNDEFINED, 
            PIXEL_WIDTH, PIXEL_HEIGHT, SDL_WINDOW_SHOWN);
        if (window == nullptr)
        {
            std::string SDL_err_code = SDL_GetError();
            std::string err_string = "SDL_CreateWindow Error: ";
            throw std::runtime_error(err_string + SDL_err_code);
        }
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (renderer == nullptr)
        {
            std::string SDL_err_code = SDL_GetError();
            std::string err_string = "SDL_CreateRenderer Error: ";
            throw std::runtime_error(err_string + SDL_err_code);
        }
        screen_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, PIXEL_WIDTH, PIXEL_HEIGHT);
        byte_framebuffer.resize(PIXEL_WIDTH * PIXEL_HEIGHT);
        palettes.resize(81, 0);
    }
    is_on = true;
}

void Screen::turn_off()
{
    SDL_DestroyTexture(screen_texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    window = nullptr;

    // quit SDL
    SDL_Quit();

    is_on = false;
}

/*
void Screen::read_tilemap(std::string const& filename)
{
    std::ifstream tilemap_stream(filename);
    size_t i = 0;
    while (i < 6561)
    {
        tilemap_stream >> tilemap[i];
        i++;
    }
    return;
}

void Screen::show_tilemap()
{
    // first, read tilemap's middle tile into tryte_frame_buffer with palette 0
    for (size_t j = 0; j < 6561; j++)
    {
        tryte_framebuffer[j] = -364 + (j % 729);
    }

    // let's set a palette colour
    work_RAM[PALETTE_START + 39] = Tryte("mMM"); // negative trit pixels will be red
    work_RAM[PALETTE_START + 40] = Tryte("MmM"); // zero trit pixels will be green
    work_RAM[PALETTE_START + 41] = Tryte("MMm"); // positive trit pixels will be blue

    // cache the palettes as a byte array
    regen_palettes();

    // event loop
    SDL_Event e;
    bool quit = false;
    while (!quit)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
            if (e.type == SDL_KEYDOWN)
            {
                quit = true;
            }
            if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                quit = true;
            }
            write_tryte_fb_to_byte_fb();
            SDL_UpdateTexture(screen_texture, nullptr, byte_framebuffer.data(), PIXEL_WIDTH * sizeof(uint32_t));
            SDL_RenderCopy(renderer, screen_texture, nullptr, nullptr);
            SDL_RenderPresent(renderer);
            SDL_Delay(100);
        }
    }
    return;
}
*/

void Screen::write_tryte_fb_to_byte_fb()
{
    // fetch framebuffer ready for copying
    MemoryBlock* framebuffer = gpu.get_framebuffer();
    // loop through tryte framebuffer
    for (int64_t i = 0; i < TILE_GRID_HEIGHT; i++)
    {
        for (int64_t j = 0; j < TILE_GRID_WIDTH; j++)
        {
            Tryte& tile = (*framebuffer)[(TILE_GRID_WIDTH * i) + j];
            size_t palette_index = tile.get_high() + 13;
            size_t tile_addr = static_cast<size_t>(27 * static_cast<int64_t>(tile.get_mid()) + static_cast<int64_t>(tile.get_low()) + 364);
            write_tile_to_framebuffer(j, i, palette_index, tile_addr);
        }
    }
}

void Screen::write_tile_to_framebuffer(size_t const grid_index_x, size_t const grid_index_y, size_t const palette_index, size_t const tile_addr)
{
    // get palette colours
    std::array<uint32_t, 3> colours;
    colours[0] = palettes[3 * palette_index]; // -1 colour
    colours[1] = palettes[3 * palette_index + 1]; // 0 colour
    colours[2] = palettes[3 * palette_index + 2]; // +1 colour

    // find pointer to tile's trytes in tilemap
    MemoryBlock* tilemap = gpu.get_tilemap();
    Tryte* tile_trytes = &((*tilemap)[9 * tile_addr]);

    int64_t pixel_index_x = PIXELS_PER_TILE * grid_index_x;
    int64_t pixel_index_y = PIXELS_PER_TILE * grid_index_y;

    for (int64_t row = 0; row < PIXELS_PER_TILE; row++)
    {
        int64_t y = pixel_index_y + row;
        for (int64_t col = 0; col < PIXELS_PER_TILE; col++)
        {
            int64_t x = pixel_index_x + col;
            byte_framebuffer[(PIXEL_WIDTH * y) + x] = colours[tile_trytes[row / PIXELS_PER_TRIT][8 - (col / PIXELS_PER_TRIT)] + 1];
        }
    }
}

void Screen::regen_palettes()
{
    MemoryBlock* work_RAM = gpu.get_work_RAM();
    for (int64_t j = 0; j < 27; j++)
    {
        Tryte& neg_colour = (*work_RAM)[PALETTE_START + 3 * j];
        Tryte& zero_colour = (*work_RAM)[PALETTE_START + (3 * j) + 1];
        Tryte& pos_colour = (*work_RAM)[PALETTE_START + (3 * j) + 2];

        palettes[3 * j] = tryte_to_colour(neg_colour);
        palettes[3 * j + 1] = tryte_to_colour(zero_colour);
        palettes[3 * j + 2] = tryte_to_colour(pos_colour);
    }
}

uint32_t Screen::tryte_to_colour(Tryte const& colour_tryte)
{
    uint32_t red = colour_tryte.get_high() + 13;
    uint32_t green = colour_tryte.get_mid() + 13;
    uint32_t blue = colour_tryte.get_low() + 13;

    return 0xFF000000 | (colour_values[red] << 16) | (colour_values[green] << 8) | colour_values[blue];
}

void Screen::run()
{
    // event loop
    SDL_Event e;
    while (is_on)
    {
        while (SDL_PollEvent(&e))
        {
            uint64_t frame_start = SDL_GetPerformanceCounter();

            // handle input
            if (e.type == SDL_QUIT)
            {
                turn_off();
            }
            /*
            if (e.type == SDL_KEYDOWN)
            {
                switch (e.key.keysym.sym)
                {
                    case SDLK_f:
                        dump_bank(tryte_framebuffer);
                        break;
                    case SDLK_t:
                        dump_bank(tilemap);
                        break;
                    default:
                        break;
                }
            }
            */
            if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                turn_off();
            }

            regen_palettes();
            draw_to_screen();
            uint64_t frame_end = SDL_GetPerformanceCounter();

            // calculate elapsed time in milliseconds
            double elapsed_time = static_cast<double>(frame_end - frame_start) * 1000.0 / static_cast<double>(SDL_GetPerformanceFrequency());

            double wait_time = 1000.0 / FPS - elapsed_time;
            if (wait_time < 0)
            {
                wait_time = 0;
            }

            // cap frame rate at 30 fps
            SDL_Delay(static_cast<uint32_t>(std::floor(wait_time)));
        }
    }
    return;
}

void Screen::draw_to_screen()
{
    write_tryte_fb_to_byte_fb();
    SDL_UpdateTexture(screen_texture, nullptr, byte_framebuffer.data(), PIXEL_WIDTH * sizeof(uint32_t));
    SDL_RenderCopy(renderer, screen_texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}

/*
void Screen::dump_bank(Bank const& bank)
{
    size_t const ROW_LENGTH = 27;
    Tryte row_start = -9841;
    Tryte row_end = -9841 + ROW_LENGTH - 1;
    // draw the top row
    std::cout << "      M   L   K   J   I   H   G   F   E   D   C   B   A   0   a   b   c";
    std::cout << "   d   e   f   g   h   i   j   k   l   m     \n";
    for (size_t i = 0; i < 243; i++)
    {
        std::cout << Tryte(row_start + (ROW_LENGTH * i)) << ": ";
        for (size_t j = 0; j < ROW_LENGTH; j++)
        {
            std::cout << bank[ROW_LENGTH * i + j] << ' ';
        }
        std::cout << ":" << Tryte(row_end + (ROW_LENGTH * i)) << "\n";
    }
    std::cout << "\n\n";

}
*/
