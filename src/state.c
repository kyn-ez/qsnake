#include "headers/state.h"
#include "headers/settings.h"

static void randomize_position(Position* position)
{
    position->x = SDL_rand(CELLS_IN_WINDOW_SIDE);
    position->y = SDL_rand(CELLS_IN_WINDOW_SIDE);
}

bool positions_are_equal(Position position1, Position position2)
{
    const bool same_x = position1.x == position2.x;
    const bool same_y = position1.y == position2.y;
    return (same_x && same_y);
}

void snake_add_new_body_part(State* state)
{
    const size_t last_index = state->snake.body_length++;
    if (last_index > 0) {
        state->snake.body[last_index] = state->snake.body[last_index - 1];
    } else {
        state->snake.body[last_index] = state->snake.head_position;
    }
}

void spawn_new_apple(State* state)
{
    bool valid;

    do {
        randomize_position(&state->apple_position);
        valid = !positions_are_equal(state->apple_position, state->snake.head_position);
        for (size_t i = 0; i < state->snake.body_length && valid; i++) {
            valid = !positions_are_equal(state->apple_position, state->snake.body[i]);
        }
    } while (!valid);
}

void start_new_game(State* state)
{
    spawn_new_apple(state);
    randomize_position(&state->snake.head_position);
    state->snake.head_direction = SDL_rand(NUMBER_OF_DIRECTIONS);
    state->snake.body_length = 0;
    state->score = 0;
}