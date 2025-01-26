#include <iostream>
#include <random>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "Game.hpp"

Game::Game() : current_direction(Direction::NONE), previous_time(0) {}

void Game::start()
{
    is_running = true;
}

void Game::stop()
{
    is_running = false;
}

void Game::restart()
{
    init_snake();
    generate_apple();
    is_game_over = false;
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

    if (font)
    {
        TTF_CloseFont(font);
        font = nullptr;
    }

    if (text_texture)
    {
        SDL_DestroyTexture(text_texture);
        text_texture = nullptr;
    }

    SDL_Quit();
}

Game::~Game()
{
    clean();
}

void Game::init_snake()
{
    if (!snake.empty())
    {
        snake.clear();
    }

    SDL_Rect part;
    part.x = WIDTH / 2 - CELL_SIZE;
    part.y = HEIGHT / 2 - CELL_SIZE;
    part.w = CELL_SIZE;
    part.h = CELL_SIZE;
    snake.push_back(part);
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

    if (TTF_Init() < 0)
    {
        std::cerr << "Failed to initialize SDL TTF: " << TTF_GetError() << std::endl;
        SDL_Quit();
        return false;
    }

    move_delay = DEFAULT_MOVE_DELAY;

    font = TTF_OpenFont(FONT_PATH.c_str(), FONT_SIZE);

    if (!font)
    {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
        return false;
    }

    text_color = {255, 255, 255, 255};
    text_surface = TTF_RenderText_Solid(font, "Game Over!", text_color);

    if (!text_surface)
    {
        std::cerr << "Failed to create the text surface:" << SDL_GetError() << std::endl;
        return false;
    }

    text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
    if (!text_texture)
    {
        std::cerr << "Failed to create the text texture:" << SDL_GetError() << std::endl;
        return false;
    }

    is_game_over = false;

    text_rect.x = 50;
    text_rect.y = 50;
    text_rect.w = text_surface->w;
    text_rect.h = text_surface->h;

    init_snake();
    generate_apple();

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
        if (current_direction != Direction::LEFT)
        {
            next_direction = Direction::RIGHT;
        }
        break;
    case SDLK_LEFT:
        if (current_direction != Direction::RIGHT)
        {
            next_direction = Direction::LEFT;
        }
        break;
    case SDLK_UP:
        if (current_direction != Direction::DOWN)
        {
            next_direction = Direction::UP;
        }
        break;
    case SDLK_DOWN:
        if (current_direction != Direction::UP)
        {
            next_direction = Direction::DOWN;
        }
        break;
    case SDLK_SPACE:
        if (is_game_over)
        {
            restart();
            break;
        }

        if (key.state == SDL_PRESSED)
        {
            move_delay = SPED_UP_MOVE_DELAY;
        }
        else if (key.state == SDL_RELEASED)
        {
            move_delay = DEFAULT_MOVE_DELAY;
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
        if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP)
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

void Game::generate_apple()
{
    const int row_num = HEIGHT / CELL_SIZE;
    const int col_num = WIDTH / CELL_SIZE;

    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<unsigned int> x_uniform(0, col_num - 1);
    std::uniform_int_distribution<unsigned int> y_uniform(0, row_num - 1);

    unsigned int x_rand = 0;
    unsigned int y_rand = 0;

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

    apple.x = x_rand;
    apple.y = y_rand;
    apple.w = CELL_SIZE;
    apple.h = CELL_SIZE;
}

bool Game::is_collision()
{
    if (snake.size() < 2)
    {
        return false;
    }

    SDL_Rect head = snake[0];
    for (int i = 1; i < snake.size(); ++i)
    {
        if (head.x == snake[i].x && head.y == snake[i].y)
        {
            return true;
        }
    }

    return false;
}

void Game::move()
{
    SDL_Rect &head = snake.front();
    SDL_Rect head_prev = snake.front();
    SDL_Rect tail_prev = snake.back();

    switch (current_direction)
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

    if (is_collision())
    {
        is_game_over = true;
        return;
    }

    if (head.x == apple.x && head.y == apple.y)
    {
        snake.push_back(tail_prev);
        generate_apple();
    }
}

void Game::play()
{
    if (current_direction == Direction::NONE)
    {
        current_direction = next_direction;
        return;
    }

    Uint32 current_time = SDL_GetTicks();
    if (current_time > previous_time + move_delay)
    {
        current_direction = next_direction;
        move();
        previous_time = current_time;
    }
}

void Game::render()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    if (is_game_over)
    {
        SDL_RenderCopy(renderer, text_texture, NULL, &text_rect);
    }
    else
    {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderDrawRect(renderer, &apple);
        SDL_RenderFillRect(renderer, &apple);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
        for (const auto &part : snake)
        {
            SDL_RenderFillRect(renderer, &part);
            SDL_RenderDrawRect(renderer, &part);
        }
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
