#ifndef QSNAKE_AGENT_H
#define QSNAKE_AGENT_H

#include "game.h"

// A simplified state representation for the AI agent
typedef struct {
    // Normalized vector representing the direction to the apple.
    // Can be one of -1, 0, 1 for each dimension
    Vec2d apple_vector;

    // Danger (if there's a body part) in each direction
    bool danger_up;
    bool danger_down;
    bool danger_left;
    bool danger_right;

    // The current direction of snake's head
    Direction current_direction;
} State;

void setup_agent(Agent* agent);
void lobotomize_agent(Agent* agent);
void toggle_agent(Agent* agent, Game* game);

State get_agent_state(Game* game);
int encode_state(State state);

Direction choose_action(Agent* agent, State state);
void learn(Agent* agent, Direction action, State old_state, State new_state, float reward, bool game_over);
void execute_action(Agent* agent, Game* game);

bool save_model(Agent* agent, const char* file_path);
bool load_model(Agent* agent, const char* file_path);

#endif