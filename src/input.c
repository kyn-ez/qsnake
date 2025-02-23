#include <SDL3/SDL.h>

#include "state.h"

// Handle all the supported keyboard inputs
static SDL_AppResult handle_keydown(State* state, SDL_Scancode key_code)
{
    switch (key_code) {
        case SDL_SCANCODE_W:
            state->snake_direction = MOVE_UP;
            break;
        case SDL_SCANCODE_S:
            state->snake_direction = MOVE_DOWN;
            break;
        case SDL_SCANCODE_A:
            state->snake_direction = MOVE_LEFT;
            break;
        case SDL_SCANCODE_D:
            state->snake_direction = MOVE_RIGHT;
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