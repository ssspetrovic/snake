# Snake | SDL2/C++

## Linux Instructions

### Dependency Installation

```bash
sudo apt install libsdl2-dev
```

### Running the Game

Clone the repository:
```bash
git clone https://github.com/ssspetrovic/snake.git
cd snake
```

Compile the binary:
```bash
g++ -o snake main.cpp Game.cpp `sdl2-config --cflags --libs`
```

Run the game:
```bash
./snake
```

## Windows 10/11 Instructions

Prerequisites: mingw64 and g++

### Setting up the environment

Clone the environment:

```bash
git clone https://github.com/ssspetrovic/snake.git
cd snake
```

Download the SDL binaries from https://github.com/libsdl-org/SDL/releases. Tested version can be downloaded from this link: https://github.com/libsdl-org/SDL/releases/download/release-2.30.11/SDL2-devel-2.30.11-mingw.zip/.

Copy the `bin`, `include`, `lib` and `share` directories from the extracted archive into the root of the repository.

Build the executable:

```bash
g++ -I./include -L./lib -o snake.exe Game.cpp main.cpp -lmingw32 -lSDL2main -lSDL2 -mwindows
```

Run the game:

```ps
.\snake.exe
```

## Playing the game

Control the snake using the arrow keys (&larr;, &uarr;, &rarr; or &darr;) and try to eat the 🍎!
<br/>
Use `ESC` key or click on the &#10006; button to exit the game.
