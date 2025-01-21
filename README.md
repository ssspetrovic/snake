# Snake | SDL2/C++

## Linux Instructions

### Dependency Installation

```bash
sudo apt install libsdl2-2.0-0
```

### Running the Game

```bash
# Clone the repo
git clone git@github.com:ssspetrovic/snake.git
cd snake

# Compile the binary
g++ -o snake.out snake.cpp Game.cpp `sdl2-config --cflags --libs`

# Run the game
./snake.out
```
