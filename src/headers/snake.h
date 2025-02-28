#ifndef QSNAKE_SNAKE_H
#define QSNAKE_SNAKE_H

#include "game.h"

void check_if_the_apple_was_eaten(Game* game);
void snake_add_new_body_part(Game* game);
void snake_propose_new_direction(Game* game, Direction proposed);
void update_snake_position(Game* game);
bool snake_has_collided(Game* game);

#endif