#pragma once

#include "typedefs.h"

#include <map>
#include <queue>
#include <unordered_map>
#include <utility>
#include <vector>

using FrontierVec = std::vector<GridNode>;
using CameFrom = std::map<grid_location<int>, grid_location<int>>;

std::pair<FrontierVec, CameFrom> GreedySearch(const grid_world& grid,
                                              const grid_location<int>& start,
                                              const grid_location<int>& goal,
                                              int step_limit)
{
    FrontierVec frontier;
    CameFrom came_from;
    std::unordered_map<grid_location<int>, int> visited; // track if node is already in frontier

    // push start node
    frontier.push_back(GridNode{.location = start, .cost = 0});
    came_from[start] = {};
    visited[start] = 0;

    int steps = 0;
    while (!frontier.empty() && steps++ < step_limit)
    {
        // sort frontier by cost (heuristic only)
        std::sort(frontier.begin(), frontier.end(),
                  [](const GridNode& a, const GridNode& b)
                  { return a.cost < b.cost; });

        // pop lowest cost node
        GridNode current = frontier.front();
        frontier.erase(frontier.begin());

        if (current.location == goal)
            break;

        // expand neighbors
        for (const auto& next : grid.neighbors(current.location))
        {
            if (!visited.contains(next))
            {
                int cost = ManhattanDistance(goal, next);
                frontier.push_back(GridNode{.location = next, .cost = cost});
                came_from[next] = current.location;
                visited[next] = cost;
            }
        }
    }

    return {frontier, came_from};
}