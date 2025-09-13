#pragma once

#include "datastructures/grid_world.h"
#include "typedefs.h"

#include <map>
#include <queue>
#include <unordered_map>
#include <utility>
#include <vector>

using CameFrom = std::map<grid_location<int>, grid_location<int>>;

std::pair<std::priority_queue<GridNode>, CameFrom> AStarSearch(const grid_world& grid, const grid_location<int>& start, const grid_location<int>& goal, int step_limit)
{
    std::unordered_map<grid_location<int>, int> cost_so_far;
    std::priority_queue<GridNode> frontier;
    CameFrom came_from;

    frontier.push((GridNode){.cost = 0, .location = start});
    came_from[start] = {};
    cost_so_far[start] = 0;

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
            auto new_cost = cost_so_far[current.location] + 1;
            if (!cost_so_far.contains(next) || new_cost < cost_so_far[next])
            {
                auto h = ManhattanDistance(next, goal);
                auto f = new_cost + h;
                auto cost = new_cost + ManhattanDistance(next, goal);
                cost_so_far[next] = new_cost;
                frontier.push((GridNode){.cost = f, .location = next, .g = new_cost, .h = h});
                came_from[next] = current.location;
            };
        }
    }

    return {frontier, came_from};
}