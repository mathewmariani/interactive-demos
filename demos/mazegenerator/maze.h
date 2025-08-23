#pragma once

#include "datastructures/finite_grid.h"

namespace maze
{

enum class NodeType
{
    Unvisited,
    Frontier,
    Visited,
};

struct MazeNode
{
    bool north_edge;
    bool west_edge;
    NodeType type;
};

class Maze
{
  public:
    Maze() = default;

    void Resize(int size);

    MazeNode GetNode(const grid_location<int>& point);

    const NodeType GetNodeType(const grid_location<int>& point) const;
    void SetNodeType(const grid_location<int>& point, NodeType type);

    bool GetNorth(const grid_location<int>& point);
    bool GetEast(const grid_location<int>& point);
    bool GetSouth(const grid_location<int>& point);
    bool GetWest(const grid_location<int>& point);

    void SetNorth(const grid_location<int>& point, const bool& state);
    void SetEast(const grid_location<int>& point, const bool& state);
    void SetSouth(const grid_location<int>& point, const bool& state);
    void SetWest(const grid_location<int>& point, const bool& state);

    const int Size() const { return size; }
    const int Width() const { return width; }

  private:
    int size;
    int width;
    finite_grid<MazeNode> maze;
};

} // namespace maze