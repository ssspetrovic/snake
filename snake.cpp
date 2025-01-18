#include <iostream>
#include "Game.hpp"

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
