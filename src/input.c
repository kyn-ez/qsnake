#include "headers/state.h"

// The snake can't immediately move in the direction opposite to the one in which it is moving
// TODO: the player can avoid these restrictions by quickly making two keyboard inputs before the game logic is updated
static void snake_try_to_change_direction(State* state, SnakeDirection proposed)
{
    const SnakeDirection current = state->snake.head_direction;
    const bool direction_allowed = (proposed == MOVE_UP && current != MOVE_DOWN)
        || (proposed == MOVE_DOWN && current != MOVE_UP)
        || (proposed == MOVE_LEFT && current != MOVE_RIGHT)
        || (proposed == MOVE_RIGHT && current != MOVE_LEFT);

    if (direction_allowed) {
        state->snake.head_direction = proposed;
    }
}

// Handle all the supported keyboard inputs
static SDL_AppResult handle_keydown(State* state, SDL_Scancode key_code)
{
    switch (key_code) {
        // Exit the program
        case SDL_SCANCODE_Q:
        case SDL_SCANCODE_ESCAPE:
            return SDL_APP_SUCCESS;
        
        // Restart the game
        case SDL_SCANCODE_R:
            randomize_apple_position(state);
            randomize_snake_head_state(state);
            state->score = 0;
            break;

        // Movement-related input
        case SDL_SCANCODE_W:
            snake_try_to_change_direction(state, MOVE_UP);
            break;
        case SDL_SCANCODE_S:
            snake_try_to_change_direction(state, MOVE_DOWN);
            break;
        case SDL_SCANCODE_A:
            snake_try_to_change_direction(state, MOVE_LEFT);
            break;
        case SDL_SCANCODE_D:
            snake_try_to_change_direction(state, MOVE_RIGHT);
            break;
    }

    return SDL_APP_CONTINUE;
}

// This function runs when a new event (mouse input, keypresses, etc) occurs
SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
    switch (event->type) {
        case SDL_EVENT_KEY_DOWN:
            return handle_keydown((State*)appstate, event->key.scancode);
        case SDL_EVENT_QUIT:
            return SDL_APP_SUCCESS;
    }

    return SDL_APP_CONTINUE;
}