#pragma once

#include "datastructures/grid_world.h"

#include <map>
#include <utility>
#include <queue>
#include <vector>
#include <unordered_map>

struct GridNode
{
  int cost;
  grid_location<int> location;

  bool operator<(const GridNode &other) const
  {
    return cost < other.cost;
  }
};

std::unordered_map<grid_location<int>, grid_location<int>> AStarSearch(const grid_world &grid, const grid_location<int> &start, int step_limit)
{
  std::priority_queue<GridNode> frontier;
  frontier.push((GridNode){.cost = 0, .location = start});

  std::unordered_map<grid_location<int>, grid_location<int>> came_from;
  came_from.insert({start, start});

  auto i = 0;
  while (!frontier.empty() && i++ < step_limit)
  {
    /* check for visitable neighbors */
    auto current = frontier.top().location;

    auto priority = 0;
    for (const auto &next : grid.neighbors(current))
    {
      if (!came_from.contains(next))
      {
        // priority = ManhattanDistance(goal, next)
        frontier.push((GridNode){.cost = priority, .location = next});
        came_from.insert({next, current});
      };
    }

    frontier.pop();
  }

  return came_from;
}