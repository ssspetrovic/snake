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
    Game() = default;

    void start()
    {
        is_running = true;
    }

    void stop()
    {
        is_running = false;
    }

    void clean()
    {
        if (image_surface)
        {
            SDL_FreeSurface(image_surface);
            image_surface = nullptr;
        }

        if (window)
        {
            SDL_DestroyWindow(window);
            window = nullptr;
        }

        SDL_Quit();
    }

    ~Game()
    {
        clean();
    }

    bool init()
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

        // Window is successfully created
        window_surface = SDL_GetWindowSurface(window);

        image_surface = SDL_LoadBMP("img/flower.bmp");
        if (image_surface == NULL)
        {
            std::cerr << "Failed to load the image: " << SDL_GetError() << std::endl;
            return false;
        }

        return true;
    }

    void handle_key_input(SDL_KeyboardEvent key)
    {
        switch (key.keysym.sym)
        {
        case SDLK_ESCAPE:
            stop();
        }
    }

    void handle_events()
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

    void render()
    {
        SDL_BlitSurface(image_surface, NULL, window_surface, NULL);
        SDL_UpdateWindowSurface(window);
    }

    void run()
    {
        while (is_running)
        {
            handle_events();
            render();
        }
    }
};

int main(int argc, char *argv[])
{
    Game game;

    if (!game.init())
    {
        std::cerr << "Failed to initialize the game." << std::endl;
        return -1;
    }

    game.run();

    return 0;
}
