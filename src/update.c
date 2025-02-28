#include "headers/agent.h"
#include "headers/game.h"
#include "headers/settings.h"
#include "headers/snake.h"

// This function runs once per frame
SDL_AppResult SDL_AppIterate(void* appstate)
{
    Game* game = (Game*)appstate;
    const Uint64 now_time = SDL_GetTicks();

    if (game->agent.is_active) {
        // Compute the game logic for each fixed amout of time passed since last update
        while ((now_time - game->last_update_time) >= UPDATE_INTERVAL_IN_MS) {
            execute_action(&game->agent, game);
            game->last_update_time += UPDATE_INTERVAL_IN_MS;
        }
    } else {
        // Compute the game logic for each fixed amout of time passed since last update
        while ((now_time - game->last_update_time) >= UPDATE_INTERVAL_IN_MS) {
            update_snake_position(game);
            check_if_the_apple_was_eaten(game);
            if (snake_has_collided(game)) {
                start_new_game(game);
                return SDL_APP_CONTINUE;
            }
            game->last_update_time += UPDATE_INTERVAL_IN_MS;
        }
    }

    // Clear the screen
    SDL_SetRenderDrawColor(game->renderer, 20, 20, 20, 255);
    SDL_RenderClear(game->renderer);

    // Initialize the object we will use to draw the objects in the grid
    SDL_FRect rect;
    rect.w = CELL_SIDE;
    rect.h = CELL_SIDE;

    // Draw the apple
    rect.x = game->apple_position.x * CELL_SIDE;
    rect.y = game->apple_position.y * CELL_SIDE;
    SDL_SetRenderDrawColor(game->renderer, 220, 45, 20, 255);
    SDL_RenderFillRect(game->renderer, &rect);

    // Draw the body of the snake
    SDL_SetRenderDrawColor(game->renderer, 0, 180, 0, 255);
    for (size_t i = 0; i < game->snake.body_length; i++) {
        rect.x = game->snake.body[i].x * CELL_SIDE;
        rect.y = game->snake.body[i].y * CELL_SIDE;
        SDL_RenderFillRect(game->renderer, &rect);
    }

    // Draw the snake's head
    rect.x = game->snake.head_position.x * CELL_SIDE;
    rect.y = game->snake.head_position.y * CELL_SIDE;
    SDL_SetRenderDrawColor(game->renderer, 0, 220, 0, 255);
    SDL_RenderFillRect(game->renderer, &rect);

    // Draw the grid
    SDL_SetRenderDrawColor(game->renderer, 50, 50, 50, 255);
    for (int j = 0; j < CELLS_IN_WINDOW_SIDE; j++) {
        for (int i = 0; i < CELLS_IN_WINDOW_SIDE; i++) {
            rect.x = i * CELL_SIDE;
            rect.y = j * CELL_SIDE;
            SDL_RenderRect(game->renderer, &rect);
        }
    }

    // Update the screen
    SDL_RenderPresent(game->renderer);
    return SDL_APP_CONTINUE;
}