#include <SDL.h>
#include <iostream>
#include <string>
#include <fstream>
#include <array>
#include <cstdint>

#include "Screen.h"
#include "Bank.h"
#include "Tryte.h"

std::array<uint32_t, 27> Screen::colour_values = 
{ 0, 10, 20, 29, 39, 49, 59, 69, 78, 88, 98, 108, 118, 128, 137, 147, 157, 167, 177, 186,
    196, 206, 216, 226, 235, 245, 255 };

Screen::Screen(Bank& tryte_framebuffer, Bank& tilemap, Bank& work_RAM) :
    tryte_framebuffer{ tryte_framebuffer },
    tilemap{ tilemap },
    work_RAM{ work_RAM }
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
}

Screen::~Screen()
{
    SDL_DestroyTexture(screen_texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    window = nullptr;

    // quit SDL
    SDL_Quit();
}

void Screen::test()
{
    SDL_FillRect(surface, nullptr, SDL_MapRGB(surface->format, 255, 0, 0));
    SDL_UpdateWindowSurface(window);

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
        }
    }
    return;
}

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

void Screen::write_tryte_fb_to_byte_fb()
{
    // loop through tryte framebuffer
    for (size_t i = 0; i < TILE_GRID_HEIGHT; i++)
    {
        for (size_t j = 0; j < TILE_GRID_WIDTH; j++)
        {
            Tryte& t = tryte_framebuffer[(TILE_GRID_WIDTH * i) + j];
            size_t palette_index = Tryte::get_high(t) + 13;
            size_t tilemap_addr = static_cast<size_t>(27 * static_cast<int64_t>(Tryte::get_mid(t)) + static_cast<int64_t>(Tryte::get_low(t)) + 364);
            write_tile_to_framebuffer(j, i, palette_index, tilemap_addr);
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
    Tryte* tile_trytes = &(tilemap[9 * tile_addr]);

    size_t pixel_index_x = 9 * grid_index_x;
    size_t pixel_index_y = 9 * grid_index_y;

#if 0
    std::cout << "tile location: (" << pixel_index_x << ", " << pixel_index_y << ")\n";
    std::cout << "tile: ";
    for (size_t i = 0; i < 9; i++)
    {
        std::cout << tile_trytes[i] << ' ';
    }
    std::cout << '\n';
#endif

    for (size_t row = 0; row < 9; row++)
    {
        size_t y = pixel_index_y + row;
        for (size_t col = 0; col < 9; col++)
        {
            size_t x = pixel_index_x + col;
            byte_framebuffer[(PIXEL_WIDTH * y) + x] = colours[tile_trytes[row][8 - col] + 1];
        }
    }
#if 0
    for (size_t i = 0; i < 9; i++)
    {
        for (size_t j = 0; j < 9; j++)
        {
            size_t x = pixel_index_x + i;
            size_t y = pixel_index_y + j;
            size_t index = (PIXEL_WIDTH * y) + x;
            byte_framebuffer[(PIXEL_WIDTH * y) + x] = colours[tile_trytes[j][i] + 1];
        }
    }
#endif
}

void Screen::regen_palettes()
{
    for (size_t j = 0; j < 27; j++)
    {
        Tryte& neg_colour = work_RAM[3 * j];
        Tryte& zero_colour = work_RAM[3 * j + 1];
        Tryte& pos_colour = work_RAM[3 * j + 2];

        palettes[3 * j] = tryte_to_colour(neg_colour);
        palettes[3 * j + 1] = tryte_to_colour(zero_colour);
        palettes[3 * j + 2] = tryte_to_colour(pos_colour);
    }
}

uint32_t Screen::tryte_to_colour(Tryte const& colour_tryte)
{
    uint32_t red = Tryte::get_high(colour_tryte) + 13;
    uint32_t green = Tryte::get_mid(colour_tryte) + 13;
    uint32_t blue = Tryte::get_low(colour_tryte) + 13;

    return 0xFF000000 | (colour_values[red] << 16) | (colour_values[green] << 8) | colour_values[blue];
}
void show_window()
{
    // initialisation flag
    bool success = true;

    // define the window
    SDL_Window* window = nullptr;

    // the surface contained by the window
    SDL_Surface* screen_surface = nullptr;

    // the image we will load and show on the screen
    SDL_Surface* hello = nullptr;


    // initialise SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "SDL could not initialise! SDL Error: " << SDL_GetError() << '\n';
        success = false;
    }
    else
    {
        // create window
        window = SDL_CreateWindow("SDL test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 600, 600, SDL_WINDOW_SHOWN);
        if (window == nullptr)
        {
            std::cerr << "Window could not be created! SDL Error: " << SDL_GetError() << '\n';
            success = false;
        }
        else
        {
            // get window surface
            screen_surface = SDL_GetWindowSurface(window);
        }
    }

    const char* filename = "./res/chars.bmp";
    hello = SDL_LoadBMP(filename);
    if (hello == nullptr)
    {
        std::cerr << "Unable to load image " << filename << "! SDL Error: " << SDL_GetError() << '\n';
        success = false;
    }

    // apply the image
    SDL_BlitSurface(hello, nullptr, screen_surface, nullptr);

    // update the surface
    SDL_UpdateWindowSurface(window);

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
        }
    }

    // deallocate surface
    SDL_FreeSurface(hello);
    hello = nullptr;

    // destroy window
    SDL_DestroyWindow(window);
    window = nullptr;

    // quit SDL
    SDL_Quit();

}