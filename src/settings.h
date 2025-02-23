#ifndef QSNAKE_SETTINGS_H
#define QSNAKE_SETTINGS_H

#include <SDL3/SDL.h>

// Game settings
static const Uint64 UPDATE_INTERVAL_IN_MS = 1000;
static const int WINDOW_SIDE = 600;
static const int CELLS_IN_WINDOW_SIDE = 4;
static const int CELL_SIDE = WINDOW_SIDE / CELLS_IN_WINDOW_SIDE;

#endif