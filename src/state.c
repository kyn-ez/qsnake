#include "headers/state.h"
#include "headers/settings.h"

bool positions_are_equal(Position position1, Position position2)
{
    const bool same_x = position1.x == position2.x;
    const bool same_y = position1.y == position2.y;
    return (same_x && same_y);
}

void randomize_position(Position* position)
{
    position->x = SDL_rand(CELLS_IN_WINDOW_SIDE);
    position->y = SDL_rand(CELLS_IN_WINDOW_SIDE);
}

void start_new_game(State* state)
{
    randomize_position(&state->apple_position);
    randomize_position(&state->snake.head_position);
    state->snake.head_direction = SDL_rand(NUMBER_OF_DIRECTIONS);
    state->score = 0;
}