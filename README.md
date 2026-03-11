# Minecraft-Clone-cpp

This is a Minecraft Clone made in C++ and Raylib. 
As of Right Now the project only includes one chunk and few blocks list as follows.

1) Grass
2) Dirt
3) Wood
4) Cobblestone

# To install it on your device make sure you follow these steps:

## Dependencies
You need [Raylib](https://www.raylib.com/) installed before building.

## Linux
1. Install raylib: `sudo apt install libraylib-dev` (or build from source)
2. Clone the repo: `git clone https://github.com/Ayush-Bish1/Minecraft-Clone-cpp`
3. Go into the folder: `cd Minecraft-Clone-cpp`
4. Run: `bash build.sh`
5. Run the game: `./main`

## Windows
1. Install raylib from [raylib.com](https://www.raylib.com/)
2. Download the repo as ZIP → extract it
3. Open in VS Code or any IDE with MinGW/GCC
4. Compile: `g++ main.cpp -o main -lraylib -lopengl32 -lgdi32 -lwinmm`
5. Run `main.exe`

## Mac
1. Install raylib: `brew install raylib`
2. Clone the repo: `git clone https://github.com/Ayush-Bish1/Minecraft-Clone-cpp`
3. Go into the folder: `cd Minecraft-Clone-cpp`
4. Compile: `g++ main.cpp -o main -lraylib -framework OpenGL -framework Cocoa`
5. Run: `./main`

## Controls
- WASD - Move
- Mouse - Look around
- Space - Jump
- 1/2/3/4 - Select block
- Left click - Place block
- Right click - Break block

# Thank You.
