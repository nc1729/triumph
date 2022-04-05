#include <SDL.h>
#include <iostream>
#include <string>

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

    const char* filename = "./triumph/res/chars.bmp";
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