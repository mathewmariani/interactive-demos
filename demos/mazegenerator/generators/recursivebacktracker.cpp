#include "mazegenerator/generators/recursizebacktracker.h"

namespace maze
{

bool RecursiveBacktracker::Step(Maze& maze)
{
    if (!initialized)
    {
        initialized = true;
        auto start = RandomStartPoint(maze);

        /* add to frontier */
        frontier.push(start);
        maze.SetNodeType(start, NodeType::Frontier);
        return true;
    }

    if (frontier.empty())
    {
        return false;
    }

    /* check for visitable neighbors */
    auto current = frontier.top();
    auto visitables = getVisitables(maze, current);

    if (visitables.empty())
    {
        frontier.pop();
        maze.SetNodeType(current, NodeType::Visited);
        return true;
    }
    else
    {
        /* get randon visitable neighbor */
        auto i = std::uniform_int_distribution<std::size_t>(0, visitables.size() - 1)(maze::gen);
        auto next = visitables[i];

        /* add to frontier */
        frontier.push(next);
        maze.SetNodeType(next, NodeType::Frontier);

        CreatePathBetween(maze, current, next);
        return true;
    }

    return false;
}

void RecursiveBacktracker::Clear(void)
{
    initialized = false;
    while (!frontier.empty())
    {
        frontier.pop();
    }
}

std::vector<grid_location<int>> RecursiveBacktracker::getVisitables(Maze& maze, const grid_location<int>& point)
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

std::vector<grid_location<int>> RecursiveBacktracker::getVisitedNeighbors(Maze& maze, const grid_location<int>& point)
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

} // namespace maze