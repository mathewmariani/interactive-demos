#include "pathfinding.h"
#include <algorithm>

std::pair<std::vector<grid_location<int>>, std::map<grid_location<int>, grid_location<int>>>
BreadthFirstSearch(
    const grid_world& grid,
    const grid_location<int>& start,
    const grid_location<int>& goal,
    int step_limit)
{
    std::vector<grid_location<int>> frontier;
    std::map<grid_location<int>, grid_location<int>> came_from;

    frontier.push_back(start);
    came_from[start] = start;

    auto i = 0;
    while (!frontier.empty() && i++ < step_limit)
    {
        auto current = frontier.front();
        frontier.erase(frontier.begin());

        if (current == goal)
        {
            break;
        }

        for (const auto& next : grid.neighbors(current))
        {
            if (came_from.find(next) == came_from.end())
            {
                frontier.push_back(next);
                came_from[next] = current;
            }
        }
    }

    return {frontier, came_from};
}