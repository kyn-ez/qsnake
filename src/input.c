#include <SDL3/SDL.h>

#include "state.h"

// The snake can't immediately move in the direction opposite to the one in which it is moving
static void snake_try_to_change_direction(State* state, SnakeDirection proposed)
{
    const SnakeDirection current = state->snake_direction;
    const bool direction_allowed = (proposed == MOVE_UP && current != MOVE_DOWN)
        || (proposed == MOVE_DOWN && current != MOVE_UP)
        || (proposed == MOVE_LEFT && current != MOVE_RIGHT)
        || (proposed == MOVE_RIGHT && current != MOVE_LEFT);

    if (direction_allowed) {
        state->snake_direction = proposed;
    }
}

// Handle all the supported keyboard inputs
static SDL_AppResult handle_keydown(State* state, SDL_Scancode key_code)
{
    switch (key_code) {
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