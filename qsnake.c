#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

// Game settings
const Uint64 UPDATE_RATE_IN_MS = 125;
const int WINDOW_SIDE = 600;
const int CELLS_IN_WINDOW_SIDE = 4;
const int CELL_SIDE = WINDOW_SIDE / CELLS_IN_WINDOW_SIDE;

// Describes the current status of the game
typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
    Uint64 last_update_time;
} State;

// This function runs once at startup
SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{
    // Initialize the SDL library
    bool ok = SDL_Init(SDL_INIT_VIDEO);
    if (!ok) {
        SDL_Log("Couldn't initialize the SDL video subsystem: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    // Allocate enough memory for the game state
    State* state = (State*)SDL_calloc(1, sizeof(State));
    if (!state) {
        SDL_Log("Coudn't allocate memory for the game state: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    // Set the SDL appstate to be our own custom game state object
    *appstate = state;

    // Create the window
    ok = SDL_CreateWindowAndRenderer("QSnake", WINDOW_SIDE, WINDOW_SIDE, SDL_WINDOW_VULKAN, &state->window, &state->renderer);
    if (!ok) {
        SDL_Log("Couldn't create window and renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    state->last_update_time = SDL_GetTicks();
    return SDL_APP_CONTINUE;
}

// This function runs once at shutdown
void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
    if (appstate) {
        State* state = (State*)appstate;
        SDL_DestroyRenderer(state->renderer);
        SDL_DestroyWindow(state->window);
        SDL_free(state);
    }
}

// This function runs when a new event (mouse input, keypresses, etc) occurs
SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
    switch (event->type) {
        case SDL_EVENT_QUIT:
            // End the program with a success status code
            return SDL_APP_SUCCESS;
        case SDL_EVENT_KEY_DOWN:
            return SDL_APP_SUCCESS;
    }

    return SDL_APP_CONTINUE;
}

// This function runs once per frame
SDL_AppResult SDL_AppIterate(void* appstate)
{
    State* state = (State*)appstate;
    const Uint64 now_time = SDL_GetTicks();

    // Compute the game logic for each fixed amout of time passed since last update
    while ((now_time - state->last_update_time) >= UPDATE_RATE_IN_MS) {
        state->last_update_time += UPDATE_RATE_IN_MS;
    }

    // Clear the screen
    SDL_SetRenderDrawColor(state->renderer, 0, 0, 0, 255);
    SDL_RenderClear(state->renderer);

    // Initialize the object we will use to draw each cell of the grid
    SDL_FRect rect;
    rect.w = WINDOW_SIDE;
    rect.h = WINDOW_SIDE;

    // Draw the grid
    SDL_SetRenderDrawColor(state->renderer, 255, 255, 255, 255);
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
