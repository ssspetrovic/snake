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
    constexpr static Uint32 DEFAULT_MOVE_DELAY = 80;
    constexpr static Uint32 SPED_UP_MOVE_DELAY = 40;

    bool is_running = true;

    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;

    std::vector<SDL_Rect> snake;
    SDL_Rect apple;
    
    Uint32 move_delay;
    Uint32 previous_time;
    Direction current_direction;
    Direction next_direction;

    // Move methods
    void move_body(SDL_Rect head_prev);
    void move_handle_margins();
    bool is_collision();
    void move();

    SDL_Rect generate_apple();

    // Event handling methods
    void handle_events(SDL_Event event);
    void handle_keyboard_event(SDL_KeyboardEvent key);

    // Main methods
    void start();
    void stop();
    void play();
    void render();
    void clean();

public:
    Game();
    ~Game();

    bool init();
    void run();
};

#endif