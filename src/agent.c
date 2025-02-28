#include <stdio.h>
#include <stdlib.h>

#include "headers/agent.h"
#include "headers/game.h"
#include "headers/snake.h"

// apple_vector.x -> 3 values (-1, 0, 1)
// apple_vector.y -> 3 values (-1, 0, 1)
// danger_up, danger_down, danger_left, danger_right -> 2 values each (true or false)
// current_direction -> 4 values (DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT)
static const int num_states = 3 * 3 * 2 * 2 * 2 * 2 * 4;

// The agent can choose between 4 actions: DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT
static const int num_actions = NUMBER_OF_DIRECTIONS;

// Setup the AI agent
void setup_agent(Agent* agent)
{
    // Allocate and initialize the Q-table
    size_t table_size = num_states * num_actions * sizeof(float);
    agent->q_table = malloc(table_size);
    memset(agent->q_table, 0, table_size);

    // Initialize all the other fields
    agent->is_active = false;
    agent->episode_count = 0;
    agent->steps_count = 0;
    agent->games_played = 0;
    agent->highest_score = 0;
    agent->average_score = 0;

    // Try to load an existing model if available
    load_model(agent, "model.bin");
}

// Save the model and make the agent unlearn everything
void lobotomize_agent(Agent* agent)
{
    if (agent->q_table) {
        save_model(agent, "model.bin");
        free(agent->q_table);
        agent->q_table = NULL;
    }
}

// Toggle the AI agent
void toggle_agent(Agent* agent, Game* game)
{
    agent->is_active = !agent->is_active;

    if (agent->is_active) {
        // Reset game when AI starts
        start_new_game(game);
        agent->steps_count = 0;
        SDL_Log("AI Agent activated. Episode: %d", agent->episode_count);
    } else {
        // Save the model when turning off
        save_model(agent, "model.bin");
        SDL_Log("AI Agent deactivated. Player has regained control.");
    }
}

// Get the state representation from the game state
State get_agent_state(Game* game)
{
    State state;
    state.current_direction = game->snake.head_direction;

    // Apple direction relative to the snake's head position
    const int dx = game->apple_position.x - game->snake.head_position.x;
    const int dy = game->apple_position.y - game->snake.head_position.y;

    // Normalize to -1, 0, 1
    state.apple_vector.x = (dx > 0) ? 1 : ((dx < 0) ? -1 : 0);
    state.apple_vector.y = (dy > 0) ? 1 : ((dy < 0) ? -1 : 0);

    // Check for danger in each direction
    {
        state.danger_up = false;
        state.danger_down = false;
        state.danger_left = false;
        state.danger_right = false;

        // Positions to check
        Vec2d up = { game->snake.head_position.x, game->snake.head_position.y - 1 };
        Vec2d down = { game->snake.head_position.x, game->snake.head_position.y + 1 };
        Vec2d left = { game->snake.head_position.x - 1, game->snake.head_position.y };
        Vec2d right = { game->snake.head_position.x + 1, game->snake.head_position.y };

        // Handle warping
        up.y = (up.y < 0) ? CELLS_IN_WINDOW_SIDE - 1 : up.y;
        down.y = (down.y >= CELLS_IN_WINDOW_SIDE) ? 0 : down.y;
        left.x = (left.x < 0) ? CELLS_IN_WINDOW_SIDE - 1 : left.x;
        right.x = (right.x >= CELLS_IN_WINDOW_SIDE) ? 0 : right.x;

        for (int i = 0; i < game->snake.body_length; i++) {
            state.danger_up = (positions_are_equal(up, game->snake.body[i])) ? true : state.danger_up;
            state.danger_down = (positions_are_equal(down, game->snake.body[i])) ? true : state.danger_down;
            state.danger_left = (positions_are_equal(left, game->snake.body[i])) ? true : state.danger_left;
            state.danger_right = (positions_are_equal(right, game->snake.body[i])) ? true : state.danger_right;
        }
    }

    return state;
}

// Convert state to a unique index in the Q-table
int encode_state(State state)
{
    int index = 0;
    index = state.apple_vector.x + 1;
    index = (index * 3) + (state.apple_vector.y + 1);
    index = (index * 2) + state.danger_up;
    index = (index * 2) + state.danger_down;
    index = (index * 2) + state.danger_left;
    index = (index * 2) + state.danger_right;
    index = (index * 4) + state.current_direction;
    return index;
}

// Choose the next action based on the current state (using an ε-greedy policy)
Direction choose_action(Agent* agent, State state)
{
    const int encoded_state = encode_state(state);

    // Explore: choose a random action with probability EXPLORATION_RATE
    const float random_float = (float)rand() / RAND_MAX;
    if (random_float < EXPLORATION_RATE) {
        return rand() % NUMBER_OF_DIRECTIONS;
    }

    // Exploit: choose the best action based on our current knowledge
    int best_action = 0;
    float best_value = agent->q_table[encoded_state * num_actions];

    for (int i = 0; i < num_actions; i++) {
        const float value = agent->q_table[encoded_state * num_actions + i];
        if (value > best_value) {
            best_value = value;
            best_action = i;
        }
    }

    return best_action;
}

// Update the Q-table based on the results of the latest action
void learn(Agent* agent, Direction action, State old_state, State new_state, float reward, bool game_over)
{
    const int encoded_old_state = encode_state(old_state);
    const int encoded_new_state = encode_state(new_state);

    // Get the current value for the state-action pair
    const float current_value = agent->q_table[encoded_old_state * num_actions + action];

    // Find the max value for the next state
    float max_future_value = 0;
    for (int i = 0; i < num_actions; i++) {
        const float value = agent->q_table[encoded_new_state * num_actions + i];
        if (value > max_future_value) {
            max_future_value = value;
        }
    }

    // Q-learning formula: Q(s,a) = Q(s,a) + α * [r + γ * max Q(s',a') - Q(s,a)]
    float new_value;
    if (game_over) {
        // If game over, the future value doesn't matter
        new_value = current_value + LEARNING_RATE * (reward - current_value);
    } else {
        new_value = current_value + LEARNING_RATE * (reward + DISCOUNT_FACTOR * max_future_value - current_value);
    }

    // Update the Q-table
    agent->q_table[encoded_old_state * num_actions + action] = new_value;
}

// Helper function
static int min(int a, int b) {
    return (a < b) ? a : b;
}

// Perform a single step chosen by the AI agent
void execute_action(Agent* agent, Game* game)
{
    // Save current state
    const State initial_state = get_agent_state(game);
    const int initial_score = game->score;

    // Choose action based on current state
    Direction chosen_action = choose_action(agent, initial_state);

    // Update snake direction based on the chosen action
    game->snake.proposed_head_direction = chosen_action;

    // Remember position before move (to check for collision)
    const Vec2d initial_position = game->snake.head_position;

    // Update game state (run game logic)
    update_snake_position(game);
    check_if_the_apple_was_eaten(game);

    // Get new state
    const State new_state = get_agent_state(game);

    // Calculate reward
    float reward = 0;

    // Check if snake ate an apple
    if (game->score > initial_score) {
        reward = 10; // Big reward for eating apple
    }

    // Check if snake died (collided with itself)
    bool game_over = false;
    if (snake_has_collided(game)) {
        reward = -10; // Penalty for dying
        game_over = true;

        // Update statistics
        if (game->score > agent->highest_score) {
            agent->highest_score = game->score;
        }

        // Update average score
        agent->average_score = (agent->average_score * agent->games_played + game->score) / (agent->games_played + 1);
        agent->games_played++;

        // Start new game
        start_new_game(game);
        agent->episode_count++;
        agent->steps_count = 0;

        if (agent->episode_count % 100 == 0) {
            // Save model periodically
            save_model(agent, "model.bin");
            SDL_Log("Episode: %d, Average Score: %.2f, Highest Score: %d", agent->episode_count, agent->average_score, agent->highest_score);
        }

        // Check if we reached max episodes
        if (agent->episode_count >= MAX_EPISODES) {
            agent->is_active = false;
            SDL_Log("Training complete after %d episodes.", agent->episode_count);
        }

        return;
    }

    // Small reward for moving closer to the apple, penalty for moving away
    // Calculate distances before and after the move
    int initial_dx = abs(game->apple_position.x - initial_position.x);
    int initial_dy = abs(game->apple_position.y - initial_position.y);
    int new_dx = abs(game->apple_position.x - game->snake.head_position.x);
    int new_dy = abs(game->apple_position.y - game->snake.head_position.y);

    // Account for warping
    initial_dx = min(initial_dx, CELLS_IN_WINDOW_SIDE - initial_dx);
    initial_dy = min(initial_dy, CELLS_IN_WINDOW_SIDE - initial_dy);
    new_dx = min(new_dx, CELLS_IN_WINDOW_SIDE - new_dx);
    new_dy = min(new_dy, CELLS_IN_WINDOW_SIDE - new_dy);

    // Manhattan distance
    int initial_manhattan = initial_dx + initial_dy;
    int new_manhattan = new_dx + new_dy;

    if (new_manhattan < initial_manhattan) {
        reward += 0.1; // Moving closer to apple
    } else if (new_manhattan > initial_manhattan) {
        reward -= 0.1; // Moving away from apple
    }

    // Small penalty for each step (encourages efficient paths)
    reward -= 0.01;

    // Update Q-table
    learn(agent, chosen_action, initial_state, new_state, reward, game_over);

    // Increment step counter
    agent->steps_count++;

    // Add a time limit to avoid infinite games
    if (agent->steps_count > 1000) {
        start_new_game(game);
        agent->episode_count++;
        agent->steps_count = 0;
    }
}

// Save the Q-table to disk
bool save_model(Agent* agent, const char* file_path)
{
    FILE* file = fopen(file_path, "wb");
    if (!file) {
        SDL_Log("Failed to open file for writing: %s", file_path);
        return false;
    }

    // Write metadata
    fwrite(&agent->games_played, sizeof(int), 1, file);
    fwrite(&agent->highest_score, sizeof(int), 1, file);
    fwrite(&agent->average_score, sizeof(float), 1, file);
    fwrite(&agent->episode_count, sizeof(int), 1, file);

    // Write Q-table data
    const size_t table_size = num_states * num_actions;
    fwrite(agent->q_table, sizeof(float), table_size, file);

    fclose(file);
    SDL_Log("Model saved to %s", file_path);
    return true;
}

// Load the Q-table from disk
bool load_model(Agent* agent, const char* file_path)
{
    FILE* file = fopen(file_path, "rb");
    if (!file) {
        SDL_Log("No existing model found at %s, starting fresh", file_path);
        return false;
    }

    // Read metadata
    fread(&agent->games_played, sizeof(int), 1, file);
    fread(&agent->highest_score, sizeof(int), 1, file);
    fread(&agent->average_score, sizeof(float), 1, file);
    fread(&agent->episode_count, sizeof(int), 1, file);

    // Read Q-table
    const size_t table_size = num_states * num_actions;
    fread(agent->q_table, sizeof(float), table_size, file);

    fclose(file);
    SDL_Log("Model loaded from %s. Episodes: %d, Highest Score: %d", file_path, agent->episode_count, agent->highest_score);
    return true;
}