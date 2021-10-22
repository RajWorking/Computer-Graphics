#ifndef MAZE_H
#define MAZE_H

#include "game_object.h"
#include <vector>

class Maze
{
public:
    std::vector<GameObject> Walls; // collection of walls
    std::vector<std::pair<int, int>> roomGrph[100][100];
    int Len, Ht;
    int ox, oy;

    int vis[100][100];
    std::pair<int, int> dfs(std::pair<int, int> i, std::pair<int, int> search);
    std::pair<int, int> find(std::pair<int, int> from, std::pair<int, int> to);
};

#endif