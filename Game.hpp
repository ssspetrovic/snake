#ifndef GAME_HPP
#define GAME_HPP

#include <iostream>
#include <cstdlib>

#include <SDL2/SDL.h>

class Game
{
private:
    bool is_running = true;
    constexpr static int WIDTH = 800;
    constexpr static int HEIGHT = 600;

    SDL_Window *window = nullptr;
    SDL_Surface *window_surface = nullptr;
    SDL_Surface *image_surface = nullptr;
    SDL_Event event;

public:
    Game();
    ~Game();

    void start();
    void stop();
    void clean();
    bool init();
    void handle_key_input(SDL_KeyboardEvent key);
    void handle_events();
    void render();
    void run();
    SDL_Surface *optimize_surface(const std::string &filepath);
};

#endif