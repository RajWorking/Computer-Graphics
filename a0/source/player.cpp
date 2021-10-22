#include "player.h"

PlayerObject::PlayerObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec2 velocity, int Posture, glm::vec3 color)
    : GameObject(pos, size, sprite, velocity, color), posture(Posture), alive(1), score(0), death_stage(0) {}

void PlayerObject::Move()
{
    posture = 1 + posture % 60;
    std::string item = (facing ? "right" : "left");
    item += std::to_string((posture + 4) / 5);
    Sprite = ResourceManager::GetTexture(item);
}

void PlayerObject::Die()
{
    death_stage++;
    std::string item = "dead";
    item += std::to_string((death_stage + 19) / 20);
    Sprite = ResourceManager::GetTexture(item);
}

void PlayerObject::UpdateCell(int ox, int oy)
{
    int col = round((Position.x - ox - 25.0f + Size.x / 2) / 50.0f);
    int row = round((Position.y - oy - 25.0f + Size.y / 2) / 50.0f);

    cell = {col, row};

    // int border = 8.0f;

    // if (Position.x - border > ox + col * 50.0f and
    //     Position.x + Size.x + border < ox + (col + 1) * 50.0f and
    //     Position.y - border > oy + row * 50.0f and
    //     Position.y + Size.y + border < oy + (row + 1) * 50.0f)
    //     cell = {col, row};

    // if (Position.x < ox + col * 50.0f + 25.0f and
    //     Position.x + Size.x > ox + col * 50.0f + 25.0f and
    //     Position.y < oy + row * 50.0f + 25.0f and
    //     Position.y + Size.y > oy + row * 50.0f + 25.0f)
    //     cell = {col, row};
}