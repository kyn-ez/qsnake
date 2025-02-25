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
    const Position initial_head_position = state->snake.head_position;

    // Update the snake's position based on the direction we are currently taking
    switch (state->snake.head_direction) {
        case MOVE_UP: state->snake.head_position.y--; break;
        case MOVE_DOWN: state->snake.head_position.y++; break;
        case MOVE_LEFT: state->snake.head_position.x--; break;
        case MOVE_RIGHT: state->snake.head_position.x++; break;
    }

    // Warp to the other side of the grid if necessary
    warp_around_grid(&state->snake.head_position);

    // Update the rest of the snake's body if necessary
    if (state->snake.body_length > 0) {
        // Shift from tail to head
        for (int i = state->snake.body_length - 1; i > 0 - 1; i--) {
            state->snake.body[i] = state->snake.body[i - 1];
        }

        // First body part follows the head
        state->snake.body[0] = initial_head_position;
    }
}

// Check if the snake's head hasn't collided with its body
static bool snake_has_collided(State* state)
{
    for (int i = 1; i < state->snake.body_length; i++) {
        if (positions_are_equal(state->snake.body[i], state->snake.head_position))
            return true;
    }

    return false;
}

// Run the scoring logic
static void check_if_the_apple_was_eaten(State* state)
{
    if (positions_are_equal(state->snake.head_position, state->apple_position)) {
        spawn_new_apple(state);
        snake_add_new_body_part(state);
        SDL_Log("Score: %d\n", ++state->score);
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
        if (snake_has_collided(state)) {
            start_new_game(state);
            return SDL_APP_CONTINUE;
        }
        state->last_update_time += UPDATE_INTERVAL_IN_MS;
    }

    // Clear the screen
    SDL_SetRenderDrawColor(state->renderer, 20, 20, 20, 255);
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

    // Draw the body of the snake
    SDL_SetRenderDrawColor(state->renderer, 0, 180, 0, 255);
    for (size_t i = 0; i < state->snake.body_length; i++) {
        rect.x = state->snake.body[i].x * CELL_SIDE;
        rect.y = state->snake.body[i].y * CELL_SIDE;
        SDL_RenderFillRect(state->renderer, &rect);
    }

    // Draw the snake's head
    rect.x = state->snake.head_position.x * CELL_SIDE;
    rect.y = state->snake.head_position.y * CELL_SIDE;
    SDL_SetRenderDrawColor(state->renderer, 0, 220, 0, 255);
    SDL_RenderFillRect(state->renderer, &rect);

    // Draw the grid
    SDL_SetRenderDrawColor(state->renderer, 50, 50, 50, 50);
    for (int j = 0; j < CELLS_IN_WINDOW_SIDE; j++) {
        for (int i = 0; i < CELLS_IN_WINDOW_SIDE; i++) {
            rect.x = i * CELL_SIDE;
            rect.y = j * CELL_SIDE;
            SDL_RenderRect(state->renderer, &rect);
        }
    }

    // Update the screen
    SDL_RenderPresent(state->renderer);
    return SDL_APP_CONTINUE;
}