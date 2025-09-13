#pragma once

#include "typedefs.h"

#include <map>
#include <queue>
#include <unordered_map>
#include <utility>
#include <vector>

using Frontier = std::vector<grid_location<int>>;
using CameFrom = std::map<grid_location<int>, grid_location<int>>;

std::pair<std::priority_queue<GridNode>, CameFrom> GreedySearch(const grid_world& grid, const grid_location<int>& start, const grid_location<int>& goal, int step_limit)
{
    std::priority_queue<GridNode> frontier;
    CameFrom came_from;

    frontier.push((GridNode){.cost = 0, .location = start});
    came_from[start] = {};

    auto i = 0;
    while (!frontier.empty() && i++ < step_limit)
    {
        auto current = frontier.top();
        frontier.pop();

        if (current.location == goal)
        {
            break;
        }

        /* check for visitable neighbors */
        for (const auto& next : grid.neighbors(current.location))
        {
            if (!came_from.contains(next))
            {
                auto cost = ManhattanDistance(goal, next);
                frontier.push((GridNode){.cost = cost, .location = next});
                came_from[next] = current.location;
            };
        }
    }

    return {frontier, came_from};
}