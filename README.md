# qsnake
An AI agent that learns to play *snake* using the *Q-learning* algorithm. The game is built using **SDL3** for graphics and **CMake** for the build system. This project demonstrates how reinforcement learning can be applied to a classic game.

## Prerequisites
Before building the project, ensure that you have the following dependencies installed:
- **CMake** >= 3.16
- **SDL3**

You can get **SDL3** by using the Git submodule (see the [build instructions](#build-instructions)). Make sure you have all the necessary build dependencies installed. For more information, refer to the [SDL3 documentation](https://github.com/libsdl-org/SDL/blob/main/docs).

## Build Instructions
Follow these steps to build and run the project:

### 1. Clone the repository

```bash
git clone https://github.com/kyn-ez/qsnake.git
cd qsnake
```

### 2. Initialize Git Submodules
```bash
git submodule update --init
```

### 3. Build the project
```bash
# Configure the project
cmake -S . -B build

# Build the executable
cmake --build build
```

### 4. Run the game
```bash
./build/Debug/qsnake
```