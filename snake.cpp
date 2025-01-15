#include <iostream>
#include <cstdlib>

#include <SDL2/SDL.h>

constexpr int WIDTH = 800;
constexpr int HEIGHT = 600;

int main(int argc, char const *argv[])
{
    SDL_Window *window = nullptr;

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "Failed to initialize SDL video" << std::endl;
        exit(EXIT_FAILURE);
    }
    
    window = SDL_CreateWindow("snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    
    if (window == NULL)
    {
        std::cerr << "Failed to create the window: " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    SDL_UpdateWindowSurface(window);
    SDL_Delay(5000);

    return 0;
}
