#include <iostream>

#include <SDL2/SDL.h>
#include "Game.hpp"

Game::Game() = default;

void Game::start()
{
    is_running = true;
}

void Game::stop()
{
    is_running = false;
}

void Game::clean()
{
    if (drawing_rect)
    {
        delete drawing_rect;
        drawing_rect = nullptr;
    }

    if (image_surface)
    {
        SDL_FreeSurface(image_surface);
        image_surface = nullptr;
    }

    if (window_surface)
    {
        SDL_FreeSurface(window_surface);
        window_surface = nullptr;
    }

    if (window)
    {
        SDL_DestroyWindow(window);
        window = nullptr;
    }

    SDL_Quit();
}

Game::~Game()
{
    clean();
}

bool Game::init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "Failed to initialize SDL video" << std::endl;
        return false;
    }

    window = SDL_CreateWindow("snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);

    if (!window)
    {
        std::cerr << "Failed to create the window: " << SDL_GetError() << std::endl;
        return false;
    }

    // Window is successfully created
    window_surface = SDL_GetWindowSurface(window);

    image_surface = optimize_surface("img/flower.bmp");
    if (image_surface == NULL)
    {
        std::cerr << "Failed to optimize the surface" << std::endl;
        return false;
    }

    drawing_rect = new SDL_Rect;
    drawing_rect->x = drawing_rect->y = 0;
    drawing_rect->w = WIDTH;
    drawing_rect->h = HEIGHT;

    return true;
}

void Game::handle_key_input(SDL_KeyboardEvent key)
{
    switch (key.keysym.sym)
    {
    case SDLK_ESCAPE:
        stop();
    }
}

void Game::handle_events()
{
    while (SDL_PollEvent(&event) != 0)
    {
        if (event.type == SDL_QUIT)
        {
            stop();
        }

        if (event.type == SDL_KEYDOWN)
        {
            handle_key_input(event.key);
        }
    }
}

void Game::render()
{
    SDL_BlitScaled(image_surface, NULL, window_surface, drawing_rect);
    SDL_UpdateWindowSurface(window);
}

void Game::run()
{
    while (is_running)
    {
        handle_events();
        render();
    }
}

SDL_Surface *Game::optimize_surface(const std::string &filepath)
{
    SDL_Surface *optimized_surface = nullptr;

    // Load the image from the specified file
    SDL_Surface *image_surface = SDL_LoadBMP(filepath.c_str());
    if (image_surface == nullptr)
    {
        std::cerr << "Failed to load the image: " << SDL_GetError() << std::endl;
        return nullptr;
    }

    optimized_surface = SDL_ConvertSurface(image_surface, window_surface->format, 0);
    if (optimized_surface == nullptr)
    {
        std::cerr << "Failed to convert the surface: " << SDL_GetError() << std::endl;
    }

    SDL_FreeSurface(image_surface);

    return optimized_surface;
}
