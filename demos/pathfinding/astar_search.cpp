#include "pathfinding.h"
#include <algorithm>

std::pair<std::vector<GridNode>, std::map<grid_location<int>, grid_location<int>>>
AStarSearch(
    const grid_world& grid,
    const grid_location<int>& start,
    const grid_location<int>& goal,
    int step_limit)
{
    std::vector<GridNode> frontier;
    std::unordered_map<grid_location<int>, int> g_score;
    std::map<grid_location<int>, grid_location<int>> came_from;

    auto h = [&](const grid_location<int>& a)
    { return ManhattanDistance(a, goal); };

    GridNode startNode{.cost = 0, .location = start};
    frontier.push_back(startNode);
    g_score[start] = 0;

    int steps = 0;
    while (!frontier.empty() && steps++ < step_limit)
    {
        // sort entries; product of using std::vector
        std::sort(frontier.begin(), frontier.end());

        GridNode current = frontier.front();
        frontier.erase(frontier.begin());

        if (current.location == goal)
        {
            break;
        }

        for (auto& next : grid.neighbors(current.location))
        {
            int new_g = current.g + 1;
            if (!g_score.count(next) || new_g < g_score[next])
            {
                g_score[next] = new_g;
                GridNode neighborNode{
                    .location = next,
                    .g = new_g,
                    .h = h(next),
                    .cost = new_g + h(next),
                };

                // remove old entries; product of using std::vector
                frontier.erase(
                    std::remove_if(frontier.begin(), frontier.end(), [&](const GridNode& n)
                                   { return n.location == next; }),
                    frontier.end());

                frontier.push_back(neighborNode);
                came_from[next] = current.location;
            }
        }
    }

    return {frontier, came_from};
}