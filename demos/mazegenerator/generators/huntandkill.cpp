#include "mazegenerator/generators/huntandkill.h"

namespace maze
{

bool HuntAndKill::Step(Maze& maze)
{
    if (!initialized)
    {
        initialized = true;
        auto point = RandomStartPoint(maze);

        /* add to frontier */
        frontier.push(point);
        maze.SetNodeType(point, NodeType::Frontier);
        return true;
    }

    if (frontier.empty())
    {
        std::vector<grid_location<int>> visitables{};
        grid_location<int> parent{0, 0};
        grid_location<int> next{0, 0};

        for (auto j = 0; j < maze.Width(); ++j)
        {
            for (auto i = 0; i < maze.Width(); ++i)
            {
                parent = {i, j};
                if (maze.GetNodeType(parent) == NodeType::Visited)
                {
                    visitables = getVisitables(maze, parent);
                    if (visitables.empty())
                    {
                        continue;
                    }

                    auto k = std::uniform_int_distribution<std::size_t>(0, visitables.size() - 1)(maze::gen);
                    auto next = visitables[k];

                    CreatePathBetween(maze, parent, next);

                    /* add to frontier */
                    frontier.push(next);
                    maze.SetNodeType(next, NodeType::Frontier);
                    return true;
                }
            }
        }
    }
    else
    {
        /* pick a random point from frontier */
        auto parent = frontier.front();

        /* remove point from frontier */
        maze.SetNodeType(parent, NodeType::Visited);
        frontier.pop();

        auto visitables = getVisitables(maze, parent);
        if (!visitables.empty())
        {
            auto i = std::uniform_int_distribution<std::size_t>(0, visitables.size() - 1)(maze::gen);
            auto next = visitables[i];

            CreatePathBetween(maze, parent, next);

            /* add to frontier */
            frontier.push(next);
            maze.SetNodeType(next, NodeType::Frontier);
        }
        return true;
    }

    return false;
}

void HuntAndKill::Clear(Maze& maze)
{
    initialized = false;
    while (!frontier.empty())
    {
        frontier.pop();
    }
}

std::vector<grid_location<int>> HuntAndKill::getVisitables(Maze& maze, const grid_location<int>& point)
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

std::vector<grid_location<int>> HuntAndKill::getVisitedNeighbors(Maze& maze, const grid_location<int>& point)
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