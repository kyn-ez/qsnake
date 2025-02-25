#include "headers/settings.h"
#include "headers/state.h"

// When the snake goes beyond the borders of the grid, warp it to the other side
static void warp_around_grid(Position* position)
{
    position->x = (position->x < 0) ? CELLS_IN_WINDOW_SIDE - 1 : position->x % CELLS_IN_WINDOW_SIDE;
    position->y = (position->y < 0) ? CELLS_IN_WINDOW_SIDE - 1 : position->y % CELLS_IN_WINDOW_SIDE;
}

// Run the movement logic for the snake
static void update_snake_position(State* state)
{
    // Update the snake's position based on the direction we are currently taking
    switch (state->snake.head_direction) {
        case MOVE_UP:
            state->snake.head_position.y--;
            break;
        case MOVE_DOWN:
            state->snake.head_position.y++;
            break;
        case MOVE_LEFT:
            state->snake.head_position.x--;
            break;
        case MOVE_RIGHT:
            state->snake.head_position.x++;
            break;
    }

    // Warp to the other side of the grid if necessary
    warp_around_grid(&state->snake.head_position);
}

// Run the scoring logic
// TODO: the apple shouldn't be able to spawn inside the snake
static void check_if_the_apple_was_eaten(State* state)
{
    if (positions_are_equal(state->snake.head_position, state->apple_position)) {
        SDL_Log("Score: %d\n", ++state->score);
        randomize_position(&state->apple_position);
    }
}

// This function runs once per frame
SDL_AppResult SDL_AppIterate(void* appstate)
{
    State* state = (State*)appstate;
    const Uint64 now_time = SDL_GetTicks();

    // Compute the game logic for each fixed amout of time passed since last update
    while ((now_time - state->last_update_time) >= UPDATE_INTERVAL_IN_MS) {
        update_snake_position(state);
        check_if_the_apple_was_eaten(state);
        state->last_update_time += UPDATE_INTERVAL_IN_MS;
    }

    // Clear the screen
    SDL_SetRenderDrawColor(state->renderer, 200, 100, 50, 255);
    SDL_RenderClear(state->renderer);

    // Initialize the object we will use to draw the objects in the grid
    SDL_FRect rect;
    rect.w = CELL_SIDE;
    rect.h = CELL_SIDE;

    // Draw the apple
    rect.x = state->apple_position.x * CELL_SIDE;
    rect.y = state->apple_position.y * CELL_SIDE;
    SDL_SetRenderDrawColor(state->renderer, 220, 45, 20, 255);
    SDL_RenderFillRect(state->renderer, &rect);

    // Draw the snake
    rect.x = state->snake.head_position.x * CELL_SIDE;
    rect.y = state->snake.head_position.y * CELL_SIDE;
    SDL_SetRenderDrawColor(state->renderer, 25, 230, 20, 255);
    SDL_RenderFillRect(state->renderer, &rect);

    // Update the screen
    SDL_RenderPresent(state->renderer);
    return SDL_APP_CONTINUE;
}