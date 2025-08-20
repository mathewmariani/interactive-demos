#pragma once

#include "mazegenerator/generators/mazegenerator.h"

#include <vector>

namespace maze
{

class RandomWalk : public MazeGenerator
{
  public:
    RandomWalk() = default;
    std::string GetName() override { return "Random Walk"; };

    bool Step(Maze& maze) override;
    void Clear(Maze& maze) override;

  private:
    std::vector<grid_location<int>> getVisitables(Maze& maze, const grid_location<int>& point);
    std::vector<grid_location<int>> getVisitedNeighbors(Maze& maze, const grid_location<int>& point);

  private:
    bool initialized = false;
    std::vector<grid_location<int>> frontier;
};

}