#ifndef QSNAKE_STATE_H
#define QSNAKE_STATE_H

#include <SDL3/SDL.h>

// The input directions the player can give to move the snake
typedef enum {
    MOVE_UP,
    MOVE_DOWN,
    MOVE_LEFT,
    MOVE_RIGHT,
    NUMBER_OF_DIRECTIONS,
} SnakeDirection;

// Fields related to the snake
typedef struct {
    int head_position_x;
    int head_position_y;
    SnakeDirection head_direction;
} SnakeState;

// Fields related to the apple
typedef struct {
    int position_x;
    int position_y;
} AppleState;

// Describes the current status of the game
typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
    Uint64 last_update_time;
    int score;
    SnakeState snake;
    AppleState apple;
} State;

void randomize_apple_position(State* state);
void randomize_snake_head_state(State* state);

#endif