#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "headers/settings.h"
#include "headers/game.h"
#include "headers/agent.h"

// This function runs once at startup
SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{
    // Initialize the SDL library
    bool is_video_initialized = SDL_Init(SDL_INIT_VIDEO);
    if (!is_video_initialized) {
        SDL_Log("Couldn't initialize the SDL video subsystem: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    // Allocate enough memory for the game state
    Game* game = (Game*)SDL_calloc(1, sizeof(Game));
    if (!game) {
        SDL_Log("Coudn't allocate memory for the game state: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    // Set the SDL appstate to be our own custom game state object
    *appstate = game;

    // Create the window
    bool is_window_created = SDL_CreateWindowAndRenderer("QSnake", WINDOW_SIDE, WINDOW_SIDE, SDL_WINDOW_VULKAN, &game->window, &game->renderer);
    if (!is_window_created) {
        SDL_Log("Couldn't create window and renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    // Initialize randomically the fields related to the apple and the snake
    start_new_game(game);

    // Initialize the AI agent
    setup_agent(&game->agent);

    game->last_update_time = SDL_GetTicks();
    return SDL_APP_CONTINUE;
}

// This function runs once at shutdown
void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
    if (appstate) {
        Game* game = (Game*)appstate;
        lobotomize_agent(&game->agent);
        SDL_DestroyRenderer(game->renderer);
        SDL_DestroyWindow(game->window);
        SDL_free(game);
    }
}