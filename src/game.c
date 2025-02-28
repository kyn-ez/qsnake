#include "headers/game.h"
#include "headers/settings.h"

static void randomize_position(Vec2d* position)
{
    position->x = SDL_rand(CELLS_IN_WINDOW_SIDE);
    position->y = SDL_rand(CELLS_IN_WINDOW_SIDE);
}

bool positions_are_equal(Vec2d position1, Vec2d position2)
{
    const bool same_x = position1.x == position2.x;
    const bool same_y = position1.y == position2.y;
    return (same_x && same_y);
}

void spawn_new_apple(Game* game)
{
    bool valid;

    do {
        randomize_position(&game->apple_position);
        valid = !positions_are_equal(game->apple_position, game->snake.head_position);
        for (size_t i = 0; i < game->snake.body_length && valid; i++) {
            valid = !positions_are_equal(game->apple_position, game->snake.body[i]);
        }
    } while (!valid);
}

void start_new_game(Game* game)
{
    spawn_new_apple(game);
    randomize_position(&game->snake.head_position);
    game->snake.proposed_head_direction = SDL_rand(NUMBER_OF_DIRECTIONS);
    game->snake.body_length = 0;
    game->score = 0;
}