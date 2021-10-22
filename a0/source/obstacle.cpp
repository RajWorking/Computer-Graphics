#include "obstacle.h"

Obstacle::Obstacle(glm::vec2 pos, glm::vec2 size, Texture2D sprite, int score)
    : GameObject(pos, size, sprite), value(score) {}
