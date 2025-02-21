#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;

// This function runs once at startup
SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{
    // Create the window
    bool ok = SDL_CreateWindowAndRenderer("QSnake", 800, 600, SDL_WINDOW_VULKAN, &window, &renderer);
    if (!ok) {
        SDL_Log("Couldn't create window and renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    return SDL_APP_CONTINUE;
}

// This function runs when a new event (mouse input, keypresses, etc) occurs
SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
    if (event->type == SDL_EVENT_KEY_DOWN || event->type == SDL_EVENT_QUIT) {
        // End the program with a success status code
        return SDL_APP_SUCCESS;
    }

    return SDL_APP_CONTINUE;
}

// This function runs once per frame
SDL_AppResult SDL_AppIterate(void* appstate)
{
    const char* message = "Hello, World!";

    // Get the output size in pixels
    int w = 0, h = 0;
    SDL_GetRenderOutputSize(renderer, &w, &h);

    // Set the rendering scale
    const float scale = 4.0f;
    SDL_SetRenderScale(renderer, scale, scale);

    // Center the message
    float x = ((w / scale) - SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE * SDL_strlen(message)) / 2;
    float y = ((h / scale) - SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE) / 2;

    // Clear the screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Draw the text
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDebugText(renderer, x, y, message);

    // Update the screen
    SDL_RenderPresent(renderer);
    return SDL_APP_CONTINUE;
}

// This function runs once at shutdown
void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
}
