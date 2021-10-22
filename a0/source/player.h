#ifndef PLAYEROBJECT_H
#define PLAYEROBJECT_H

#include "game_object.h"
#include "resource_manager.h"

class PlayerObject : public GameObject
{
public:
    // Player state
    bool facing; // left 0, right 1
    int posture; // posture (1,12)
    bool alive;
    int death_stage;
    int score;

    std::pair<int, int> cell;

    // constructor(s)
    PlayerObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec2 velocity, int Posture, glm::vec3 color = glm::vec3(1.0f));

    void Move();
    void Die();
    void UpdateCell(int ox, int oy);
};

#endif