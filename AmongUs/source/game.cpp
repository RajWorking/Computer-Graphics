#include "game.h"
#include "resource_manager.h"
#include "sprite_renderer.h"
#include <bits/stdc++.h>
#include "player.h"
#include "config.h"
#include "text_renderer.h"

Game::Game(unsigned int width, unsigned int height)
    : State(GAME_ACTIVE), Keys(), Width(width), Height(height)
{
    time(&time_begin);
}

Game::~Game()
{
}

SpriteRenderer *Renderer;
TextRenderer *Text;

void Game::Init()
{
    // load shaders
    ResourceManager::LoadShader("../source/shaders/sprite.vs", "../source/shaders/sprite.frag", nullptr, "sprite");
    // configure shaders
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width),
                                      static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
    // set render-specific controls
    Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
    // load textures
    ResourceManager::LoadTexture("../source/textures/background.jpg", false, "background");
    ResourceManager::LoadTexture("../source/textures/wood.png", false, "brick");
    ResourceManager::LoadTexture("../source/textures/bomb.png", false, "bomb");
    ResourceManager::LoadTexture("../source/textures/coin.png", false, "coin");
    ResourceManager::LoadTexture("../source/textures/report.png", true, "kill");
    ResourceManager::LoadTexture("../source/textures/drop.png", true, "drop");
    ResourceManager::LoadTexture("../source/textures/dead1.png", true, "dead1");
    ResourceManager::LoadTexture("../source/textures/dead2.png", true, "dead2");

    //load font
    Text = new TextRenderer(this->Width, this->Height);
    Text->Load("../source/fonts/Antonio-Bold.ttf", 24);

    for (int i = 1; i <= 12; i++)
    {
        // std::string item = "left" + std::to_string(i);
        std::string item = "left" + std::to_string(i);
        std::string file = "../source/textures/walk/" + item + ".png";
        ResourceManager::LoadTexture(&file[0], true, item);
        item = "right" + std::to_string(i);
        file = "../source/textures/walk/" + item + ".png";
        ResourceManager::LoadTexture(&file[0], true, item);
    }

    // create maze graph

    maze.Len = MAZE_LEN;
    maze.Ht = MAZE_HT;
    srand(time(NULL));

    for (int j = 0; j < maze.Ht; j++)
    {
        for (int i = 0; i < maze.Len; i++)
        {
            if (i == 0 and j == 0)
                continue;
            else if (i - 1 < 0)
            {
                maze.roomGrph[i][j].push_back({i, j - 1});
                maze.roomGrph[i][j - 1].push_back({i, j});
            }
            else if (j - 1 < 0)
            {
                maze.roomGrph[i][j].push_back({i - 1, j});
                maze.roomGrph[i - 1][j].push_back({i, j});
            }
            else
            {
                if (rand() % 2)
                {
                    maze.roomGrph[i][j].push_back({i - 1, j});
                    maze.roomGrph[i - 1][j].push_back({i, j});
                }
                else
                {
                    maze.roomGrph[i][j].push_back({i, j - 1});
                    maze.roomGrph[i][j - 1].push_back({i, j});
                }
            }
        }
    }

    // debug
    std::ofstream myfile("maze.txt");
    for (int i = 0; i < maze.Ht; i++)
    {
        for (int j = 0; j < maze.Len; j++)
        {
            for (auto k : maze.roomGrph[i][j])
                myfile << i << " " << j << " " << k.first << " " << k.second << "\n";
        }
    }

    // create walls
    maze.ox = 150.0f, maze.oy = 100.0f; // origin
    for (int i = 0; i < maze.Len; i++)
    {
        maze.Walls.push_back(GameObject(glm::vec2(maze.ox + i * 50.0f, maze.oy), glm::vec2(50.0f, 5.0f), ResourceManager::GetTexture("brick")));
        maze.Walls.push_back(GameObject(glm::vec2(maze.ox + i * 50.0f, maze.oy + 50.0f * maze.Ht), glm::vec2(50.0f, 5.0f), ResourceManager::GetTexture("brick")));
    }
    for (int i = 0; i < maze.Ht; i++)
    {
        maze.Walls.push_back(GameObject(glm::vec2(maze.ox, maze.oy + i * 50.0f), glm::vec2(5.0f, 55.0f), ResourceManager::GetTexture("brick")));
    }
    for (int i = 0; i < maze.Ht - 1; i++)
    {
        maze.Walls.push_back(GameObject(glm::vec2(maze.ox + 50.0f * maze.Len, maze.oy + i * 50.0f), glm::vec2(5.0f, 55.0f), ResourceManager::GetTexture("brick")));
    }

    for (int j = 0; j < maze.Ht; j++)
    {
        for (int i = 0; i < maze.Len; i++)
        {
            if (i + 1 < maze.Len and
                std::find(maze.roomGrph[i][j].begin(),
                          maze.roomGrph[i][j].end(),
                          std::make_pair(i + 1, j)) == maze.roomGrph[i][j].end())
                maze.Walls.push_back(GameObject(glm::vec2(maze.ox + (i + 1) * 50.0f, maze.oy + j * 50.0f), glm::vec2(5.0f, 55.0f), ResourceManager::GetTexture("brick")));

            if (j + 1 < maze.Ht and
                std::find(maze.roomGrph[i][j].begin(),
                          maze.roomGrph[i][j].end(),
                          std::make_pair(i, j + 1)) == maze.roomGrph[i][j].end())
                maze.Walls.push_back(GameObject(glm::vec2(maze.ox + i * 50.0f, maze.oy + (j + 1) * 50.0f), glm::vec2(50.0f, 5.0f), ResourceManager::GetTexture("brick")));
        }
    }

    // player
    Player = new PlayerObject(get_pos(rand() % maze.Len, rand() % maze.Ht),
                              glm::vec2(20.0f, 30.0f),
                              ResourceManager::GetTexture("left1"),
                              glm::vec2(100.0f, 100.0f),
                              1);
    Player->UpdateCell(maze.ox, maze.oy);

    Imposter = new PlayerObject(get_pos(rand() % maze.Len, rand() % maze.Ht),
                                glm::vec2(20.0f, 30.0f),
                                ResourceManager::GetTexture("left1"),
                                glm::vec2(40.0f, 40.0f),
                                1,
                                glm::vec3(0.0f, 1.0f, 0.0f));

    Killer = new GameObject(get_pos(rand() % maze.Len, rand() % maze.Ht, glm::vec2(40.0f, 40.0f)),
                            glm::vec2(40.0f, 40.0f),
                            ResourceManager::GetTexture("kill"));

    Dropper = new GameObject(get_pos(rand() % maze.Len, rand() % maze.Ht, glm::vec2(40.0f, 40.0f)),
                             glm::vec2(40.0f, 40.0f),
                             ResourceManager::GetTexture("drop"));

    for (int i = 0; i < COINS; i++)
    {
        Obstacles.push_back(Obstacle(get_pos(rand() % maze.Len, rand() % maze.Ht, glm::vec2(40.0f, 40.0f)),
                                     glm::vec2(40.0f, 40.0f),
                                     ResourceManager::GetTexture("coin"),
                                     5));
    }
    for (int i = 0; i < BOMBS; i++)
    {
        Obstacles.push_back(Obstacle(get_pos(rand() % maze.Len, rand() % maze.Ht, glm::vec2(40.0f, 40.0f)),
                                     glm::vec2(40.0f, 40.0f),
                                     ResourceManager::GetTexture("bomb"),
                                     -5));
    }
    for (Obstacle &obj : Obstacles)
        obj.Destroyed = true; // initially invisible

    Imposter->UpdateCell(maze.ox, maze.oy);
}

glm::vec2 Game::get_pos(int col, int row, glm::vec2 shape)
{
    return glm::vec2(maze.ox + 25.0f - shape.x / 2 + 50.0f * col, maze.oy + 25.0f - shape.y / 2 + 50.0f * row);
}

void Game::ChasePlayer()
{
    std::pair<int, int> twrd = maze.find(Imposter->cell, Player->cell);
    if (twrd == Imposter->cell)
        return;

    if (twrd.first == Imposter->cell.first)
    {
        if (twrd.second > Imposter->cell.second)
        {
            Imposter->Position.y += Imposter->Velocity.y * 0.02f;
            Imposter->Move();
        }
        else if (twrd.second < Imposter->cell.second)
        {
            Imposter->Position.y -= Imposter->Velocity.y * 0.02f;
            Imposter->Move();
        }
    }
    else if (twrd.second == Imposter->cell.second)
    {
        if (twrd.first > Imposter->cell.first)
        {
            Imposter->Position.x += Imposter->Velocity.x * 0.02f;
            Imposter->facing = 1;
            Imposter->Move();
        }
        else if (twrd.first < Imposter->cell.first)
        {
            Imposter->Position.x -= Imposter->Velocity.x * 0.02f;
            Imposter->facing = 0;
            Imposter->Move();
        }
    }

    if (glm::distance(Imposter->Position, get_pos(twrd.first, twrd.second)) < 5.0f)
        Imposter->UpdateCell(maze.ox, maze.oy);

    // std::cout << "-----\n";
    // std::cout << glm::distance(Imposter->Position, get_pos(twrd.first, twrd.second)) << "\n";
    // std::cout << get_pos(twrd.first, twrd.second).x << " " << get_pos(twrd.first, twrd.second).y << "\n";
    // std::cout << Imposter->Position.x << " " << Imposter->Position.y << "\n";
    // std::cout << "...\n";
    // std::cout << twrd.first << " " << twrd.second << "\n";
    // std::cout << Player->cell.first << " " << Player->cell.second << "\n";
    // std::cout << Imposter->cell.first << " " << Imposter->cell.second << "\n";
}

void Game::Update(float dt)
{
    if (Player->Position.x > maze.ox + 50.0f * maze.Len)
    {
        State = GAME_WIN;
        return;
    }
    if (State != GAME_ACTIVE)
        return;

    // chasing
    if (Imposter->alive)
        ChasePlayer();

    // drop goods
    if (Player->alive and !Dropper->Destroyed and CheckCollision(*Player, *Dropper))
    {
        for (Obstacle &obj : Obstacles)
            obj.Destroyed = false; // initially invisible

        Dropper->Destroyed = true;
    }

    // score update
    for (Obstacle &obj : Obstacles)
    {
        if (Player->alive and !obj.Destroyed and CheckCollision(*Player, obj))
        {
            obj.Destroyed = true; // initially invisible
            Player->score += obj.value;
        }
    }

    // kill imposter
    if (Player->alive and !Killer->Destroyed and CheckCollision(*Player, *Killer))
    {
        Killer->Destroyed = true;
        Imposter->alive = false;

        for (Obstacle &obj : Obstacles)
            if (obj.value > 0)
                obj.value = 4;
            else if (obj.value < 0)
                obj.value = -7;
    }

    // kill player
    if (Imposter->alive and Player->alive and CheckCollision(*Player, *Imposter))
    {
        Player->alive = false;
    }

    // std::cout << Player->score << "\n";
}

void Game::ProcessInput(float dt)
{
    if (State != GAME_ACTIVE)
        return;

    float velocity_x = Player->Velocity.x * dt;
    float velocity_y = Player->Velocity.y * dt;
    glm::vec2 pos = Player->Position;

    if (this->Keys[GLFW_KEY_A])
    {
        if (Player->Position.x >= 0.0f)
            Player->Position.x -= velocity_x;

        Player->Move();
        Player->facing = 0;
    }
    if (this->Keys[GLFW_KEY_D])
    {
        if (Player->Position.x <= this->Width - Player->Size.x)
            Player->Position.x += velocity_x;

        Player->Move();
        Player->facing = 1;
    }
    if (this->Keys[GLFW_KEY_W])
    {
        if (Player->Position.y >= 0.0f)
            Player->Position.y -= velocity_y;

        Player->Move();
    }
    if (this->Keys[GLFW_KEY_S])
    {
        if (Player->Position.y <= this->Height - Player->Size.y)
            Player->Position.y += velocity_y;

        Player->Move();
    }

    bool collide = false;

    for (GameObject &wall : maze.Walls)
    {
        if (CheckCollision(*Player, wall))
            collide = true;
    }

    if (collide)
        Player->Position = pos;

    Player->UpdateCell(maze.ox, maze.oy);
}

bool Game::CheckCollision(GameObject &one, GameObject &two) // AABB - AABB collision
{
    // collision x-axis?
    bool collisionX = one.Position.x + one.Size.x >= two.Position.x &&
                      two.Position.x + two.Size.x >= one.Position.x;
    // collision y-axis?
    bool collisionY = one.Position.y + one.Size.y >= two.Position.y &&
                      two.Position.y + two.Size.y >= one.Position.y;
    // collision only if on both axes
    return collisionX && collisionY;
}

// void Game::Draw(SpriteRenderer &renderer)
// {
//     for (GameObject &wall : this->Maze)
//         wall.Draw(renderer);

//     this->Player->Draw(*Renderer);
// }

void Game::Render()
{
    if (State == GAME_WIN)
    {
        Text->RenderText("You WON!!", SCREEN_WIDTH / 2 - 200.0f, SCREEN_HEIGHT / 2 - 100.0f, 5.0f, glm::vec3(0.0f, 1.0f, 0.0f));
        return;
    }
    else if (State == GAME_LOST)
    {
        Text->RenderText("Sorry, you LOST.", SCREEN_WIDTH / 2 - 300.0f, SCREEN_HEIGHT / 2 - 100.0f, 5.0f, glm::vec3(1.0f, 0.0f, 0.0f));
        return;
    }

    // draw background
    Renderer->DrawSprite(ResourceManager::GetTexture("background"),
                         glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height), 0.0f);
    // Renderer->DrawSprite(ResourceManager::GetTexture("face"),
    //                      glm::vec2(275.0f, 325.0f), glm::vec2(20.0f, 30.0f), 0.0f);

    // HUD
    std::string hud;
    hud = "Score: " + std::to_string(Player->score);
    Text->RenderText(hud, maze.ox, maze.oy - 30.0f, 1.0f, glm::vec3(0.0f, 0.0f, 1.0f));

    int task_complete = std::count_if(Obstacles.begin(), Obstacles.end(),
                                      [](Obstacle o) {
                                          if (o.value > 0 and o.Destroyed)
                                              return true;
                                          return false;
                                      });
    if (!Dropper->Destroyed)
        task_complete = 0;
    hud = "Tasks: " + std::to_string(task_complete) + "/" + std::to_string(COINS);
    Text->RenderText(hud, maze.ox + 150.0f, maze.oy - 30.0f, 1.0f, glm::vec3(1.0f, 0.0f, 0.0f));

    time(&time_now);
    hud = "Time: " + std::to_string(TIME_GIVEN - (time_now - time_begin));
    Text->RenderText(hud, maze.ox + 50.0f * maze.Len - 100.0f, maze.oy - 30.0f, 1.0f, glm::vec3(1.0f, 0.0f, 1.0f));
    if (time_now - time_begin > TIME_GIVEN)
    {
        State = GAME_LOST;
        return;
    }

    for (GameObject &wall : maze.Walls)
        wall.Draw(*Renderer);

    for (Obstacle &obj : Obstacles)
        if (!obj.Destroyed)
            obj.Draw(*Renderer);

    if (!this->Killer->Destroyed)
        this->Killer->Draw(*Renderer);
    if (!this->Dropper->Destroyed)
        this->Dropper->Draw(*Renderer);

    if (Player->death_stage <= 40)
    {
        if (!Player->alive)
            Player->Die();

        this->Player->Draw(*Renderer);
    }
    else
    {
        State = GAME_LOST;
        return;
    }

    if (Imposter->death_stage <= 40)
    {
        if (!Imposter->alive)
            Imposter->Die();

        this->Imposter->Draw(*Renderer);
    }

    // this->Draw(*Renderer);
}