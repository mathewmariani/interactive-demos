#pragma once

#include "mazegenerator/generators/mazegenerator.h"

#include <queue>

namespace maze
{

class HuntAndKill : public MazeGenerator
{
  public:
    HuntAndKill() = default;
    std::string GetName() override { return "Hunt and Kill"; };

    bool Step(Maze& maze) override;
    void Clear(void) override;

  private:
    std::vector<grid_location<int>> getVisitables(Maze& maze, const grid_location<int>& point);
    std::vector<grid_location<int>> getVisitedNeighbors(Maze& maze, const grid_location<int>& point);

  private:
    bool initialized = false;
    std::queue<grid_location<int>> frontier;
};

}