#pragma once

#include "datastructures/grid_world.h"

#include <map>
#include <utility>
#include <vector>

using Frontier = std::vector<grid_location<int>>;
using CameFrom = std::map<grid_location<int>, grid_location<int>>;

std::pair<Frontier, CameFrom> BreadthFirstSearch(const grid_world &grid, const grid_location<int> &start, int step_limit)
{
  Frontier frontier;
  CameFrom came_from;

  frontier.push_back(start);
  came_from[start] = start;

  auto i = 0;
  while (!frontier.empty() && i++ < step_limit)
  {
    auto current = frontier.front();
    frontier.erase(frontier.begin());
    for (const auto &next : grid.neighbors(current))
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