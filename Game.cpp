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
    if (tex)
    {
        SDL_DestroyTexture(tex);
        tex = nullptr;
    }

    if (renderer)
    {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
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

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
    {
        std::cerr << "Failed to create a renderer" << std::endl;
        return false;
    }

    tex = load_texture("img/flower.bmp");
    if (tex == NULL)
    {
        std::cerr << "Failed to load a texture" << std::endl;
        return false;
    }

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
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, tex, NULL, NULL);
    SDL_RenderPresent(renderer);
}

void Game::run()
{
    while (is_running)
    {
        handle_events();
        render();
    }
}

SDL_Texture *Game::load_texture(const std::string &filepath)
{
    // Load the image from the specified file
    SDL_Surface *image_surface = SDL_LoadBMP(filepath.c_str());
    if (image_surface == nullptr)
    {
        std::cerr << "Failed to load the image: " << SDL_GetError() << std::endl;
        return nullptr;
    }

    tex = SDL_CreateTextureFromSurface(renderer, image_surface);
    if (tex == nullptr)
    {
        std::cerr << "Failed to convert the surface to a texture: " << SDL_GetError() << std::endl;
    }

    SDL_FreeSurface(image_surface);

    return tex;
}
