#include <iostream>
#include <random>

#include <SDL2/SDL.h>
#include "Game.hpp"

Game::Game() : previous_direction(Direction::NONE), previous_time(0) {}

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

    SDL_Rect part;
    part.x = 0;
    part.y = 0;
    part.w = CELL_SIZE;
    part.h = CELL_SIZE;
    snake.push_back(part);

    apple = generate_apple();

    return true;
}

void Game::handle_keyboard_event(SDL_KeyboardEvent key)
{
    switch (key.keysym.sym)
    {
    case SDLK_ESCAPE:
        stop();
        break;
    case SDLK_RIGHT:
        if (previous_direction != Direction::LEFT)
        {
            previous_direction = Direction::RIGHT;
        }
        break;
    case SDLK_LEFT:
        if (previous_direction != Direction::RIGHT)
        {
            previous_direction = Direction::LEFT;
        }
        break;
    case SDLK_UP:
        if (previous_direction != Direction::DOWN)
        {
            previous_direction = Direction::UP;
        }
        break;
    case SDLK_DOWN:
        if (previous_direction != Direction::UP)
        {
            previous_direction = Direction::DOWN;
        }
        break;
    }
}

void Game::handle_events(SDL_Event event)
{
    while (SDL_PollEvent(&event) != 0)
    {
        if (event.type == SDL_QUIT)
        {
            stop();
        }
        if (event.type == SDL_KEYDOWN)
        {
            handle_keyboard_event(event.key);
        }
    }
}

void Game::move_handle_margins()
{
    auto &rect_head = snake.front();
    if (rect_head.x >= WIDTH) // right margin
    {
        rect_head.x = 0;
    }
    else if (rect_head.x < 0) // left margin
    {
        rect_head.x = WIDTH - CELL_SIZE;
    }
    else if (rect_head.y >= HEIGHT) // top margin
    {
        rect_head.y = 0;
    }
    else if (rect_head.y < 0) // bottom margin
    {
        rect_head.y = HEIGHT - CELL_SIZE;
    }
}

void Game::move_body(SDL_Rect head_prev)
{
    // SDL_Rect prev_position = head_prev; // Start with the saved head position
    for (size_t i = 1; i < snake.size(); ++i)
    {
        SDL_Rect tmp = snake[i]; // Save the current segment's position
        snake[i] = head_prev;    // Update this segment to follow the previous one
        head_prev = tmp;         // Update prev_position to the saved value
    }
}

SDL_Rect Game::generate_apple()
{
    const int row_num = HEIGHT / CELL_SIZE;
    const int col_num = WIDTH / CELL_SIZE;

    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<uint> x_uniform(0, col_num - 1);
    std::uniform_int_distribution<uint> y_uniform(0, row_num - 1);

    int x_rand = -1;
    int y_rand = -1;

    bool is_apple_valid = false;
    while (!is_apple_valid)
    {
        x_rand = x_uniform(gen) * CELL_SIZE;
        y_rand = y_uniform(gen) * CELL_SIZE;

        is_apple_valid = true;
        for (const auto &part : snake)
        {
            if (x_rand == part.x && y_rand == part.y)
            {
                is_apple_valid = false;
                break;
            }
        }
    }

    // Fallback
    if (x_rand == -1 || y_rand == -1)
    {
        x_rand = 0;
        y_rand = 0;
    }

    SDL_Rect new_apple;
    new_apple.x = x_rand;
    new_apple.y = y_rand;
    new_apple.w = CELL_SIZE;
    new_apple.h = CELL_SIZE;

    return new_apple;
}

void Game::move()
{
    SDL_Rect &head = snake.front();
    SDL_Rect head_prev = snake.front();
    SDL_Rect tail_prev = snake.back();

    switch (previous_direction)
    {
    case Direction::RIGHT:
        head.x += STEP;
        break;
    case Direction::LEFT:
        head.x -= STEP;
        break;
    case Direction::UP:
        head.y -= STEP;
        break;
    case Direction::DOWN:
        head.y += STEP;
        break;
    }

    move_handle_margins();
    move_body(head_prev);
    SDL_Rect tail_new = snake.back();

    if (head.x == apple.x && head.y == apple.y)
    {
        snake.push_back(tail_prev);
        apple = generate_apple();
    }
}

void Game::play()
{
    if (previous_direction == Direction::NONE)
    {
        return;
    }

    Uint32 current_time = SDL_GetTicks();
    if (current_time > previous_time + MOVE_DELAY)
    {
        move();
        previous_time = current_time;
    }
}

void Game::render()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawRect(renderer, &apple);
    SDL_RenderFillRect(renderer, &apple);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    for (const auto &part : snake)
    {
        SDL_RenderFillRect(renderer, &part);
        SDL_RenderDrawRect(renderer, &part);
    }

    SDL_RenderPresent(renderer);
}

void Game::run()
{
    SDL_Event event;
    previous_time = SDL_GetTicks();

    while (is_running)
    {
        handle_events(event);
        play();
        render();
    }
}
