#ifndef QSNAKE_STATE_H
#define QSNAKE_STATE_H

#include "settings.h"

// Small helper struct used to represent positions in the grid
typedef struct {
    int x, y;
} Position;

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
    Position head_position;
    SnakeDirection head_direction;
    int body_length;
    Position body[CELLS_IN_WINDOW_SIDE * CELLS_IN_WINDOW_SIDE];
} SnakeState;

// Describes the current status of the game
typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
    Uint64 last_update_time;
    int score;
    SnakeState snake;
    Position apple_position;
} State;

bool positions_are_equal(Position position1, Position position2);
void snake_add_new_body_part(State* state);
void spawn_new_apple(State* state);
void start_new_game(State* state);

#endif