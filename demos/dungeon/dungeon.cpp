#include "dungeon.h"
#include <random>

namespace Dungeon
{

World::World()
{
    rooms = {};
    layout = {};
    queue = {};
    endings = {};
    started = false;
    count = 0;
    max = 15;
    min = 7;
}

void World::Generate(void)
{
    // init:
    started = true;

    // start:

    auto flag = false;
    while (!flag)
    {
        flag = Step();
    }
}

bool World::Step(void)
{
    if (!started)
    {
        return false;
    }
    return true;
}

void World::Visit(const grid_location<int>& location, const RoomType type)
{
    if (rooms.at(location) != RoomType::Null)
    {
        return;
    }

    if (CountNeighbors(location) > 1 || count >= max)
    {
        return;
    }

    rooms[location] = type;
    count++;
}

int World::CountNeighbors(const grid_location<int>& location) const
{
    auto count = 0;
    for (const auto& dt : location.VonNewmanNeighborhood)
    {
        auto loc = dt + location;
    }
    return count;
}

bool World::IsSpawn(const grid_location<int>& location) const
{
    return rooms.at(location) == RoomType::Spawn;
}

bool World::IsNormal(const grid_location<int>& location) const
{
    return rooms.at(location) == RoomType::Normal;
}

bool World::IsItem(const grid_location<int>& location) const
{
    return rooms.at(location) == RoomType::Item;
}

bool World::IsShop(const grid_location<int>& location) const
{
    return rooms.at(location) == RoomType::Shop;
}

bool World::IsSecret(const grid_location<int>& location) const
{
    return rooms.at(location) == RoomType::Secret;
}

bool World::IsBoss(const grid_location<int>& location) const
{
    return rooms.at(location) == RoomType::Boss;
}

} // namespace Dungeon