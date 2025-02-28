#ifndef QSNAKE_STATE_H
#define QSNAKE_STATE_H

#include "settings.h"

// Fields related to the Q-learning algorithm
typedef struct Agent {
    float* q_table; // Maps state-action pairs to values

    bool is_active; // Whether the AI is currently active
    int episode_count; // Number of the current training episode
    int steps_count; // Steps taken in the current episode

    // Interesting statistics
    int games_played;
    int highest_score;
    float average_score;
} Agent;

// Small helper struct used to represent positions in the grid (among other things)
typedef struct {
    int x, y;
} Vec2d;

// Small helper enum used to represent directions in the grid
typedef enum {
    DIR_UP,
    DIR_DOWN,
    DIR_LEFT,
    DIR_RIGHT,
    NUMBER_OF_DIRECTIONS,
} Direction;

// Fields related to the snake
typedef struct {
    Vec2d head_position;
    Direction head_direction;
    Direction proposed_head_direction;
    int body_length;
    Vec2d body[CELLS_IN_WINDOW_SIDE * CELLS_IN_WINDOW_SIDE];
} Snake;

// Describes the current status of the game
typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
    Uint64 last_update_time;
    int score;
    Snake snake;
    Vec2d apple_position;
    Agent agent;
} Game;

bool positions_are_equal(Vec2d position1, Vec2d position2);
void spawn_new_apple(Game* game);
void start_new_game(Game* game);

#endif