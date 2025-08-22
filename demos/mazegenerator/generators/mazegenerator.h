#pragma once

#include "mazegenerator/maze.h"

#include <random>
#include <string>

namespace maze
{

static std::random_device rd;
static std::mt19937 gen(rd());

class MazeGenerator
{
  public:
    MazeGenerator() = default;
    virtual ~MazeGenerator() = default;

    virtual std::string GetName() = 0;

    virtual bool Step(Maze& maze) = 0;
    virtual void Clear(void) = 0;

  protected:
    inline grid_location<int> RandomStartPoint(const Maze& maze) const
    {
        return {
            std::uniform_int_distribution<int>(0, maze.Width())(maze::gen),
            std::uniform_int_distribution<int>(0, maze.Width())(maze::gen)};
    }

    inline bool IsPositionInBounds(const Maze& maze, const grid_location<int>& point) const
    {
        return (point.x >= 0) && (point.x < maze.Width()) && (point.y >= 0) && (point.y < maze.Width());
    }

    inline void CreatePathBetween(Maze& maze, const grid_location<int>& parent, const grid_location<int>& next)
    {
        if (next == (parent + grid_location<int>::West))
        {
            maze.SetEast(next, false);
        }
        else if (next == (parent + grid_location<int>::South))
        {
            maze.SetSouth(next, false);
        }
        else if (next == (parent + grid_location<int>::East))
        {
            maze.SetWest(next, false);
        }
        else if (next == (parent + grid_location<int>::North))
        {
            maze.SetNorth(next, false);
        }
    }
};

} // namespace maze