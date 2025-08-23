#include "mazegenerator/maze.h"

namespace maze
{

void Maze::Resize(int new_width)
{
    width = new_width;
    auto padded_width = width + 1;
    size = padded_width * padded_width;

    maze.resize(padded_width);

    MazeNode* node;
    for (auto j = 0; j < padded_width; ++j)
    {
        for (auto i = 0; i < padded_width; ++i)
        {
            node = &maze.at({i, j});
            node->north_edge = (i < width) ? true : false;
            node->west_edge = (j < width) ? true : false;
            node->type = NodeType::Unvisited;
        }
    }
}

MazeNode Maze::GetNode(const grid_location<int>& point)
{
    return maze.at(point);
}

const NodeType Maze::GetNodeType(const grid_location<int>& point) const
{
    return maze.at(point).type;
}

void Maze::SetNodeType(const grid_location<int>& point, NodeType type)
{
    maze.at(point).type = type;
}

#define NORTH_OF(point) maze.at(point).north_edge
#define EAST_OF(point) maze.at(point + grid_location<int>::East).west_edge
#define SOUTH_OF(point) maze.at(point + grid_location<int>::North).north_edge
#define WEST_OF(point) maze.at(point).west_edge

bool Maze::GetNorth(const grid_location<int>& point)
{
    return NORTH_OF(point);
}

bool Maze::GetEast(const grid_location<int>& point)
{
    return EAST_OF(point);
}

bool Maze::GetSouth(const grid_location<int>& point)
{
    return SOUTH_OF(point);
}

bool Maze::GetWest(const grid_location<int>& point)
{
    return WEST_OF(point);
}

void Maze::SetNorth(const grid_location<int>& point, const bool& state)
{
    NORTH_OF(point) = state;
}

void Maze::SetEast(const grid_location<int>& point, const bool& state)
{
    EAST_OF(point) = state;
}

void Maze::SetSouth(const grid_location<int>& point, const bool& state)
{
    SOUTH_OF(point) = state;
}

void Maze::SetWest(const grid_location<int>& point, const bool& state)
{
    WEST_OF(point) = state;
}

} // namespace maze