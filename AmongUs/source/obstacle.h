#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "game_object.h"
#include "resource_manager.h"

class Obstacle : public GameObject
{
public:
    int value;    

    // constructor(s)
    Obstacle(glm::vec2 pos, glm::vec2 size, Texture2D sprite, int score);
};

#endif