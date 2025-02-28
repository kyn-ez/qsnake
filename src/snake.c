#include "headers/snake.h"

// Run the scoring logic
void check_if_the_apple_was_eaten(Game* game)
{
    if (positions_are_equal(game->snake.head_position, game->apple_position)) {
        spawn_new_apple(game);
        snake_add_new_body_part(game);
        SDL_Log("Score: %d\n", ++game->score);
    }
}

// Try to propose a new direction for the snake
void snake_propose_new_direction(Game* game, Direction proposed)
{
    // If the AI agent is active, don't allow the player to control the snake
    if (game->agent.is_active)
        return;

    game->snake.proposed_head_direction = proposed;
}

// When the snake goes beyond the borders of the grid, warp it to the other side
static void warp_around_grid(Vec2d* position)
{
    position->x = (position->x < 0) ? CELLS_IN_WINDOW_SIDE - 1 : position->x % CELLS_IN_WINDOW_SIDE;
    position->y = (position->y < 0) ? CELLS_IN_WINDOW_SIDE - 1 : position->y % CELLS_IN_WINDOW_SIDE;
}

// Run the movement logic for the snake
void update_snake_position(Game* game)
{
    // The snake can't immediately move in the direction opposite to the one in which it is moving
    const Direction current = game->snake.head_direction;
    const Direction proposed = game->snake.proposed_head_direction;
    const bool direction_allowed = (proposed == DIR_UP && current != DIR_DOWN)
        || (proposed == DIR_DOWN && current != DIR_UP)
        || (proposed == DIR_LEFT && current != DIR_RIGHT)
        || (proposed == DIR_RIGHT && current != DIR_LEFT);

    if (direction_allowed) {
        game->snake.head_direction = proposed;
    }

    const Vec2d initial_head_position = game->snake.head_position;

    // Update the snake's position based on the direction we are currently taking
    switch (game->snake.head_direction) {
        case DIR_UP: game->snake.head_position.y--; break;
        case DIR_DOWN: game->snake.head_position.y++; break;
        case DIR_LEFT: game->snake.head_position.x--; break;
        case DIR_RIGHT: game->snake.head_position.x++; break;
    }

    // Warp to the other side of the grid if necessary
    warp_around_grid(&game->snake.head_position);

    // Update the rest of the snake's body if necessary
    if (game->snake.body_length > 0) {
        // Shift from tail to head
        for (int i = game->snake.body_length - 1; i > 0 - 1; i--) {
            game->snake.body[i] = game->snake.body[i - 1];
        }

        // First body part follows the head
        game->snake.body[0] = initial_head_position;
    }
}

// Check if the snake's head hasn't collided with its body
bool snake_has_collided(Game* game)
{
    for (int i = 1; i < game->snake.body_length; i++) {
        if (positions_are_equal(game->snake.body[i], game->snake.head_position))
            return true;
    }

    return false;
}

void snake_add_new_body_part(Game* game)
{
    const size_t last_index = game->snake.body_length++;
    if (last_index > 0) {
        game->snake.body[last_index] = game->snake.body[last_index - 1];
    } else {
        game->snake.body[last_index] = game->snake.head_position;
    }
}