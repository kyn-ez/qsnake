#include "state.h"
#include "settings.h"

void randomize_apple_position(State* state)
{
    state->apple.position_x = SDL_rand(CELLS_IN_WINDOW_SIDE);
    state->apple.position_y = SDL_rand(CELLS_IN_WINDOW_SIDE);
}

void randomize_snake_head_state(State* state)
{
    state->snake.head_position_x = SDL_rand(CELLS_IN_WINDOW_SIDE);
    state->snake.head_position_y = SDL_rand(CELLS_IN_WINDOW_SIDE);
    state->snake.head_direction = SDL_rand(NUMBER_OF_DIRECTIONS);
}