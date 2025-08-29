#pragma once

#include "datastructures/grid_world.h"

#include <map>
#include <queue>
#include <unordered_map>
#include <utility>
#include <vector>

using Frontier = std::vector<grid_location<int>>;
using CameFrom = std::map<grid_location<int>, grid_location<int>>;

std::pair<Frontier, CameFrom> DijkstraSearch(const grid_world& grid, const grid_location<int>& start, int step_limit)
{
    Frontier frontier;
    CameFrom came_from;

    auto i = 0;
    while (!frontier.empty() && i++ < step_limit)
    {
        /* check for visitable neighbors */
        auto current = frontier.front();

        for (const auto& next : grid.neighbors(current))
        {
            if (!came_from.contains(next))
            {
                frontier.push_back(next);
                came_from.insert({next, current});
            };
        }

        frontier.erase(frontier.begin());
    }

    return {frontier, came_from};
}