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
    SDL_Event event;
    SDL_Renderer *renderer = nullptr;
    SDL_Texture *tex = nullptr;

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
    SDL_Texture *load_texture(const std::string &filepath);
};

#endif