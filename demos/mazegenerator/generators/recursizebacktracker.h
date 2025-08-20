#pragma once

#include "mazegenerator/generators/mazegenerator.h"

#include <stack>

namespace maze
{

class RecursiveBacktracker : public MazeGenerator
{
  public:
    RecursiveBacktracker() = default;
    std::string GetName() override { return "Recursive Backtracker"; };

    bool Step(Maze& maze) override;
    void Clear(Maze& maze) override;

  private:
    std::vector<grid_location<int>> getVisitables(Maze& maze, const grid_location<int>& point);
    std::vector<grid_location<int>> getVisitedNeighbors(Maze& maze, const grid_location<int>& point);

  private:
    bool initialized = false;
    std::stack<grid_location<int>> frontier;
};

}