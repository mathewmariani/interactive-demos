#include "mazegenerator/generators/randomwalk.h"

namespace maze
{

bool RandomWalk::Step(Maze& maze)
{
    if (!initialized)
    {
        initialized = true;
        auto point = RandomStartPoint(maze);

        /* add to frontier */
        frontier.push_back(point);
        maze.SetNodeType(point, NodeType::Frontier);
        return true;
    }

    /* pick a random point from frontier */
    auto i = std::uniform_int_distribution<std::size_t>(0, frontier.size() - 1)(maze::gen);
    auto next = frontier[i];

    /* remove point from frontier */
    maze.SetNodeType(next, NodeType::Visited);
    frontier.erase(frontier.begin() + i);

    auto visitables = getVisitables(maze, next);
    for (auto node : visitables)
    {
        /* add to frontier */
        frontier.push_back(node);
        maze.SetNodeType(node, NodeType::Frontier);
    }

    /* get all visited neighbors of next */
    auto neighbors = getVisitedNeighbors(maze, next);
    if (!neighbors.empty())
    {
        /* pick a random visited neighbor */
        auto j = std::uniform_int_distribution<std::size_t>(0, neighbors.size() - 1)(maze::gen);
        auto parent = neighbors[j];

        CreatePathBetween(maze, parent, next);
    }

    if (frontier.size() == 0)
    {
        return false;
    }

    return true;
}

void RandomWalk::Clear(Maze& maze)
{
    frontier.clear();
    initialized = false;
}

std::vector<grid_location<int>> RandomWalk::getVisitables(Maze& maze, const grid_location<int>& point)
{
    std::vector<grid_location<int>> visitables{};
    grid_location<int> next{0, 0};
    for (const auto& delta : grid_location<int>::VonNewmanNeighborhood)
    {
        next = point + delta;
        if (IsPositionInBounds(maze, next) && (maze.GetNodeType(next) == NodeType::Unvisited))
        {
            visitables.push_back(next);
        }
    }

    return visitables;
}

std::vector<grid_location<int>> RandomWalk::getVisitedNeighbors(Maze& maze, const grid_location<int>& point)
{
    std::vector<grid_location<int>> neighbors{};
    grid_location<int> next{0, 0};
    for (const auto& delta : grid_location<int>::VonNewmanNeighborhood)
    {
        next = point + delta;
        if (IsPositionInBounds(maze, next) && (maze.GetNodeType(next) == NodeType::Visited))
        {
            neighbors.push_back(next);
        }
    }

    return neighbors;
}

}