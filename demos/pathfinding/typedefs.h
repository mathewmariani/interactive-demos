#pragma once

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