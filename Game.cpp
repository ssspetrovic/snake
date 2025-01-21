#include <iostream>

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
    Game::clean();
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

    int x = 0;
    for (int i = 0; i < RECT_LEN; ++i)
    {
        rects[i].x = x;
        rects[i].y = 0;
        rects[i].w = CELL_SIZE;
        rects[i].h = CELL_SIZE;
        x += STEP;
    }

    return true;
}

void Game::move_right()
{
    rects[RECT_LEN - 1].x += STEP;
}

void Game::move_left()
{
    rects[RECT_LEN - 1].x -= STEP;
}

void Game::move_up()
{
    rects[RECT_LEN - 1].y -= STEP;
}

void Game::move_down()
{
    rects[RECT_LEN - 1].y += STEP;
}

void Game::handle_keyboard_event(SDL_KeyboardEvent key)
{
    switch (key.keysym.sym)
    {
    case SDLK_ESCAPE:
        Game::stop();
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
            Game::stop();
        }
        if (event.type == SDL_KEYDOWN)
        {
            Game::handle_keyboard_event(event.key);
        }
    }
}

void Game::move_handle_margins()
{
    SDL_Rect rect_head = rects[RECT_LEN - 1];
    if (rect_head.x >= WIDTH) // right margin
    {
        rect_head.x = 0;
    }

    if (rect_head.x < 0) // left margin
    {
        rect_head.x = WIDTH - CELL_SIZE;
    }

    if (rect_head.y >= HEIGHT) // top margin
    {
        rect_head.y = 0;
    }

    if (rect_head.y < 0) // bottom margin
    {
        rect_head.y = HEIGHT - CELL_SIZE;
    }
    rects[RECT_LEN - 1] = rect_head;
}

void Game::move_body(SDL_Rect head_prev)
{
    for (int i = RECT_LEN - 2; i >= 0; --i)
    {
        SDL_Rect temp = rects[i];
        rects[i] = head_prev;
        head_prev = temp;
    }
}

void Game::move()
{
    SDL_Rect head_prev = rects[RECT_LEN - 1];

    switch (previous_direction)
    {
    case Direction::RIGHT:
        Game::move_right();
        break;
    case Direction::LEFT:
        Game::move_left();
        break;
    case Direction::UP:
        Game::move_up();
        break;
    case Direction::DOWN:
        Game::move_down();
        break;
    }

    Game::move_handle_margins();
    Game::move_body(head_prev);
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
        Game::move();
        previous_time = current_time;
    }
}

void Game::render()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRects(renderer, rects, RECT_LEN);
    SDL_RenderDrawRects(renderer, rects, RECT_LEN);

    SDL_RenderPresent(renderer);
}

void Game::run()
{
    SDL_Event event;
    previous_time = SDL_GetTicks();

    while (is_running)
    {
        Game::handle_events(event);
        Game::play();
        Game::render();
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
