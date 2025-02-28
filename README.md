# qsnake

An AI agent that learns to play *Snake* using the *Q-learning* algorithm. The game is built using **SDL3** for graphics and **CMake** for the build system. This project demonstrates the application of reinforcement learning to a classic game.

![screenshot](https://github.com/user-attachments/assets/3043d173-6fa2-4bfd-9147-45c765d552b7)

---

## Features
- AI that uses Q-learning to play the Snake game.
- Easily adjustable game and AI settings.
- Statistics about the AI’s performance.
- Save and load the trained AI model.

> Note: Game statistics, including the AI's score and performance, are logged to the terminal during gameplay.

The trained AI model is saved as `model.bin` and is automatically loaded the next time the game is run.

---

## Keyboard Controls
You can control the snake or toggle the AI mode using the following keyboard inputs during gameplay:

- **WASD**: Move the snake up, left, down, or right.
- **R**: Start a new game.
- **Esc**: Exit the game.
- **Q**: Toggle between manual control and AI-controlled mode.

---

## Configuration

You can adjust various game and AI settings by modifying values in the `src/headers/settings.h` file. After making changes, rebuild the project to apply the new settings.

---

## Prerequisites
Before building the project, ensure that you have the following dependencies installed:
- **CMake** >= 3.16
- **SDL3**

You can get **SDL3** by using the Git submodule (see the [build instructions](#build-instructions)). Make sure you have all the necessary build dependencies installed. For more information, refer to the [SDL3 documentation](https://github.com/libsdl-org/SDL/blob/main/docs).

---

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
