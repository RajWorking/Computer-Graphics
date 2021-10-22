#ifndef GAME_H
#define GAME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>
#include "game_object.h"
#include "player.h"
#include "maze.h"
#include "obstacle.h"

// Represents the current state of the game
enum GameState
{
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN,
    GAME_LOST,
};

// Game holds all game-related state and functionality.
// Combines all game-related data into a single class for
// easy access to each of the components and manageability.
class Game
{
public:
    // game state
    GameState State;
    bool Keys[1024];
    unsigned int Width, Height;
    time_t time_begin, time_now;

    Maze maze;
    PlayerObject *Player, *Imposter;

    // Buttons
    GameObject *Killer, *Dropper;

    // Obstacles
    std::vector<Obstacle> Obstacles;

    // render maze
    // void Draw(SpriteRenderer &renderer);

    bool CheckCollision(GameObject &one, GameObject &two);
    glm::vec2 get_pos(int row, int col, glm::vec2 shape = glm::vec2(20.0f, 30.0f));
    void ChasePlayer();

    // constructor/destructor
    Game(unsigned int width, unsigned int height);
    ~Game();
    // initialize game state (load all shaders/textures/levels)
    void Init();
    // game loop
    void ProcessInput(float dt);
    void Update(float dt);
    void Render();
};

#endif