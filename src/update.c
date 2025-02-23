#include <SDL3/SDL.h>

#include "settings.h"
#include "state.h"

// This function runs once per frame
SDL_AppResult SDL_AppIterate(void* appstate)
{
    State* state = (State*)appstate;
    const Uint64 now_time = SDL_GetTicks();

    // Compute the game logic for each fixed amout of time passed since last update
    while ((now_time - state->last_update_time) >= UPDATE_INTERVAL_IN_MS) {
        switch (state->snake_direction) {
            case MOVE_UP:
                state->snake_position_y -= 1;
                break;
            case MOVE_DOWN:
                state->snake_position_y += 1;
                break;
            case MOVE_LEFT:
                state->snake_position_x -= 1;
                break;
            case MOVE_RIGHT:
                state->snake_position_x += 1;
                break;
        }

        state->last_update_time += UPDATE_INTERVAL_IN_MS;
    }

    // Clear the screen
    SDL_SetRenderDrawColor(state->renderer, 0, 0, 0, 255);
    SDL_RenderClear(state->renderer);

    // Initialize the object we will use to draw each cell of the grid
    SDL_FRect rect;
    rect.w = CELL_SIDE;
    rect.h = CELL_SIDE;

    // Draw the snake
    rect.x = state->snake_position_x * CELL_SIDE;
    rect.y = state->snake_position_y * CELL_SIDE;
    SDL_SetRenderDrawColor(state->renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(state->renderer, &rect);

    // Update the screen
    SDL_RenderPresent(state->renderer);
    return SDL_APP_CONTINUE;
}