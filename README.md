# Snake Game

A simple Snake game written in C++ using SDL2 as the graphics library.

---

<div align="center">
    <img src="https://github.com/user-attachments/assets/42029540-74ad-45d9-9c7f-aef0b47ed443" alt="Snake Gameplay Image" width="600">
    <p><i>Gameplay Screenshot</i></p>
</div>

---

## Features

- Classic snake gameplay with edge wrapping.
- Press space to speed up 💨
- Minimalistic design and straightforward controls.
- Cross-platform compatibility (Linux and Windows).

---

## Linux Instructions

### Dependency Installation

```bash
sudo apt install libsdl2-dev libsdl2-ttf-dev
```

### Build and Run

Clone the repository:

```bash
git clone https://github.com/ssspetrovic/snake.git
cd snake
```

Compile the game:

```bash
g++ -o snake main.cpp Game.cpp `sdl2-config --cflags --libs` -lSDL2_ttf
```

Run the game:

```bash
./snake
```

---

## Windows 10/11 Instructions

Prerequisites: mingw64 and g++

### Setting up the environment

Clone the environment:

```bash
git clone https://github.com/ssspetrovic/snake.git
cd snake
```

Manually choose and download the SDL binaries from [the official SDL2 source](https://github.com/libsdl-org/SDL/releases).

If you prefer to use the tested version can be downloaded from [here](https://github.com/libsdl-org/SDL/releases/download/release-2.30.11/SDL2-devel-2.30.11-mingw.zip/).

Copy the `bin`, `include`, `lib` and `share` directories from the extracted archive into the root of the repository.

Build the executable:

```bash
g++ -I./include -L./lib -o snake.exe Game.cpp main.cpp -lmingw32 -lSDL2main -lSDL2 -mwindows
```

Run the game:

```ps
.\snake.exe
```
