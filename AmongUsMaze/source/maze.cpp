#include "maze.h"

std::pair<int, int> Maze::dfs(std::pair<int, int> i, std::pair<int, int> search)
{
    int x = i.first, y = i.second;
    vis[x][y] = 1;

    if (search == i)
        return i;

    for (auto nbr : roomGrph[x][y])
    {
        if (!vis[nbr.first][nbr.second])
        {
            // work here
            if (dfs(nbr, search) != std::make_pair(-1, -1))
                return nbr;
        }
    }

    return {-1, -1};
}

// path finding algo
std::pair<int, int> Maze::find(std::pair<int, int> from, std::pair<int, int> to)
{
    for (int i = 0; i < 100; i++)
        for (int j = 0; j < 100; j++)
            vis[i][j] = 0;

    return dfs(from, to);
}