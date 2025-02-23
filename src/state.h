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

// Describes the current status of the game
typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
    Uint64 last_update_time;
    int snake_position_x;
    int snake_position_y;
    SnakeDirection snake_direction;
} State;

#endif