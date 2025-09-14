#pragma once

#include <map>
#include <queue>
#include <unordered_map>
#include <utility>
#include <vector>

#include "datastructures/grid_world.h"

constexpr int ManhattanDistance(const grid_location<int>& a,
                                const grid_location<int>& b) noexcept
{
    return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}

struct GridNode
{
    int cost = 0;
    int h = 0;
    int g = 0;
    grid_location<int> location;

    bool operator<(const GridNode& other) const
    {
        if (cost == other.cost)
        {
            return g > other.g;
        }
        return cost < other.cost;
    }

    bool operator==(const GridNode& other) const
    {
        return location == other.location;
    }
};

std::pair<std::vector<GridNode>, std::map<grid_location<int>, grid_location<int>>>
AStarSearch(const grid_world& grid, const grid_location<int>& start, const grid_location<int>& goal, int step_limit);

std::pair<std::vector<grid_location<int>>, std::map<grid_location<int>, grid_location<int>>>
BreadthFirstSearch(const grid_world& grid, const grid_location<int>& start, const grid_location<int>& goal, int step_limit);

std::pair<std::vector<grid_location<int>>, std::map<grid_location<int>, grid_location<int>>>
DijkstraSearch(const grid_world& grid, const grid_location<int>& start, const grid_location<int>& goal, int step_limit);

std::pair<std::vector<GridNode>, std::map<grid_location<int>, grid_location<int>>>
GreedySearch(const grid_world& grid, const grid_location<int>& start, const grid_location<int>& goal, int step_limit);
