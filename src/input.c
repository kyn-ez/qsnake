#include "headers/agent.h"
#include "headers/game.h"
#include "headers/snake.h"

// Handle all the supported keyboard inputs
static SDL_AppResult handle_keydown(Game* game, SDL_Scancode key_code)
{
    switch (key_code) {
        // Exit the program
        case SDL_SCANCODE_ESCAPE:
            return SDL_APP_SUCCESS;

        // Toggle the AI agent
        case SDL_SCANCODE_Q:
            toggle_agent(&game->agent, game);
            break;

        // Restart the game (only if the AI isn't active)
        case SDL_SCANCODE_R:
            if (!game->agent.is_active) {
                start_new_game(game);
                game->score = 0;
            }
            break;

        // Movement-related input
        case SDL_SCANCODE_W: snake_propose_new_direction(game, DIR_UP); break;
        case SDL_SCANCODE_S: snake_propose_new_direction(game, DIR_DOWN); break;
        case SDL_SCANCODE_A: snake_propose_new_direction(game, DIR_LEFT); break;
        case SDL_SCANCODE_D: snake_propose_new_direction(game, DIR_RIGHT); break;
    }

    return SDL_APP_CONTINUE;
}

// This function runs when a new event (mouse input, keypresses, etc) occurs
SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
    switch (event->type) {
        case SDL_EVENT_KEY_DOWN:
            return handle_keydown((Game*)appstate, event->key.scancode);
        case SDL_EVENT_QUIT:
            return SDL_APP_SUCCESS;
    }

    return SDL_APP_CONTINUE;
}