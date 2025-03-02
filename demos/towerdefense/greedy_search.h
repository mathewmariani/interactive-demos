#pragma once

#include "datastructures/grid_world.h"

#include <map>
#include <utility>
#include <queue>
#include <vector>
#include <unordered_map>

std::unordered_map<grid_location<int>, grid_location<int>> GreedySearch(const grid_world &grid, const grid_location<int> &start, int step_limit)
{
  std::vector<grid_location<int>> frontier;
  frontier.push_back(start);

  std::unordered_map<grid_location<int>, grid_location<int>> came_from;
  came_from.insert({start, start});

  auto i = 0;
  while (!frontier.empty() && i++ < step_limit)
  {
    /* check for visitable neighbors */
    auto current = frontier.front();

    for (const auto &next : grid.neighbors(current))
    {
      if (!came_from.contains(next))
      {
        frontier.push_back(next);
        came_from.insert({next, current});
      };
    }

    frontier.erase(frontier.begin());
  }

  return came_from;
}