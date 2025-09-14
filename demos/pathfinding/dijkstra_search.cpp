#include "pathfinding.h"
#include <algorithm>

using Frontier = std::vector<grid_location<int>>;
using CameFrom = std::map<grid_location<int>, grid_location<int>>;

std::pair<std::vector<grid_location<int>>, std::map<grid_location<int>, grid_location<int>>>
DijkstraSearch(
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
        /* check for visitable neighbors */
        auto current = frontier.front();
        frontier.erase(frontier.begin());

        if (current == goal)
        {
            break;
        }

        for (const auto& next : grid.neighbors(current))
        {
            if (!came_from.contains(next))
            {
                frontier.push_back(next);
                came_from.insert({next, current});
            };
        }
    }

    return {frontier, came_from};
}