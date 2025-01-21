#ifndef GAME_HPP
#define GAME_HPP

#include <iostream>
#include <cstdlib>

#include <SDL2/SDL.h>

enum class Direction
{
    RIGHT,
    LEFT,
    DOWN,
    UP,
    NONE
};

class Game
{
private:
    bool is_running = true;
    constexpr static int WIDTH = 800;
    constexpr static int HEIGHT = 600;
    constexpr static int CELL_SIZE = 20;
    constexpr static int STEP = 20;
    constexpr static int RECT_LEN = 4;
    constexpr static Uint32 MOVE_DELAY = 400;

    SDL_Window *window = nullptr;
    SDL_Event event;
    SDL_Renderer *renderer = nullptr;
    SDL_Texture *tex = nullptr;
    SDL_Rect rects[RECT_LEN];
    Uint32 last_time;
    Uint32 current_time;
    Direction last_direction;

    void move_right();
    void move_left();
    void move_up();
    void move_down();
    void clean();
    void start();
    void stop();
    void handle_key_input(SDL_KeyboardEvent key);
    void handle_events();
    void render();
    void play();
    void handle_margins();
    void move_body(SDL_Rect head_prev);
    void move();

    SDL_Texture *load_texture(const std::string &filepath);

public:
    Game();
    ~Game();

    bool init();
    void run();
};

#endif