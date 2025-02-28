#ifndef QSNAKE_SETTINGS_H
#define QSNAKE_SETTINGS_H

#include <SDL3/SDL.h>

// Game settings
#define UPDATE_INTERVAL_IN_MS 250
#define WINDOW_SIDE 600
#define CELLS_IN_WINDOW_SIDE 10
#define CELL_SIDE (WINDOW_SIDE / CELLS_IN_WINDOW_SIDE)

// Q-learning settings
#define LEARNING_RATE 0.1f
#define DISCOUNT_FACTOR 0.9f
#define EXPLORATION_RATE 0.1f
#define MAX_EPISODES 10000

#endif