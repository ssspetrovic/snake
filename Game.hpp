#ifndef GAME_HPP
#define GAME_HPP

#include <iostream>
#include <cstdlib>
#include <vector>

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
    constexpr static int WIDTH = 800;
    constexpr static int HEIGHT = 600;
    constexpr static int CELL_SIZE = 20;
    constexpr static int STEP = 20;
    constexpr static int RECT_LEN = 4;
    constexpr static Uint32 MOVE_DELAY = 400;

    bool is_running = true;

    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
    SDL_Texture *tex = nullptr;

    std::vector<SDL_Rect> snake;
    Uint32 previous_time;
    Direction previous_direction;

    // Move methods
    void move_right();
    void move_left();
    void move_up();
    void move_down();
    void move_body(SDL_Rect head_prev);
    void move_handle_margins();
    void move();
    void extend_snake();

    // Event handling methods
    void handle_events(SDL_Event event);
    void handle_keyboard_event(SDL_KeyboardEvent key);

    // Main methods
    void start();
    void stop();
    void play();
    void render();
    void clean();

    SDL_Texture *load_texture(const std::string &filepath);

public:
    Game();
    ~Game();

    bool init();
    void run();
};

#endif