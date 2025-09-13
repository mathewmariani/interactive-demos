#pragma once

#include "datastructures/grid_world.h"

constexpr int ManhattanDistance(const grid_location<int>& a,
                                const grid_location<int>& b) noexcept
{
    return (a.x > b.x ? a.x - b.x : b.x - a.x) +
           (a.y > b.y ? a.y - b.y : b.y - a.y);
}

struct GridNode
{
    int cost = 0;
    int h = 0;
    int g = 0;
    grid_location<int> location;

    bool operator<(const GridNode& other) const
    {
        return cost > other.cost;
    }

    bool operator==(const GridNode& other) const
    {
        return location == other.location;
    }
};