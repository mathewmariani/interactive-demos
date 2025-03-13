#include "dungeon.h"
#include <random>

namespace Dungeon
{

World::World()
{
    Clear();
}

void World::Clear(void)
{
    rooms = {};
    layout = {};
    queue = {};
    endings = {};
    started = false;
    special = false;
    count = 0;
    max = 15;
    min = 7;
}

void World::Generate(void)
{
    // init:
    started = true;

    // start:
    Visit({5, 5}, RoomType::Spawn);

    auto flag = false;
    while (!flag)
    {
        flag = Step();
    }

    // meet minimums: ?
    if (count < min)
    {
        Clear();
        Generate();
    }
}

bool World::Step(void)
{
    if (!started)
    {
        printf("starting\n");
        started = true;
        Visit({5, 5}, RoomType::Spawn);
        return false;
    }
    if (!queue.empty())
    {
        auto location = queue.front();
        queue.erase(queue.begin());

        auto flag = false;
        for (const auto& dt : location.VonNewmanNeighborhood)
        {
            // auto loc = dt + location;
            flag = Visit(dt + location, RoomType::Normal) || flag;
        }

        if (!flag)
        {
            endings.push_back(location);
        }
    }
    else
    {
        rooms[RandomEndRoom()] = RoomType::Boss;
        rooms[RandomEndRoom()] = RoomType::Item;
        rooms[RandomEndRoom()] = RoomType::Shop;
        return true;
    }
    return false;
}

bool World::Visit(const grid_location<int>& location, const RoomType type)
{
    if (rooms.contains(location))
    {
        return false;
    }

    if (CountNeighbors(location) > 1 || count >= max)
    {
        return false;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> distFail(0.0f, 1.0f);

    if (type != RoomType::Spawn && (distFail(gen) < 0.5f))
    {
        return false;
    }

    // cache:
    queue.push_back(location);
    rooms[location] = type;
    count++;

    return true;
}

int World::CountNeighbors(const grid_location<int>& location) const
{
    auto count = 0;
    for (const auto& dt : location.VonNewmanNeighborhood)
    {
        if (rooms.contains(dt + location))
        {
            count++;
        }
    }
    return count;
}

const grid_location<int> World::RandomEndRoom()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, endings.size() - 1);

    auto idx = dist(gen);
    auto location = endings[idx];
    endings.erase(endings.begin() + idx);

    return location;
}

bool World::IsSpawn(const grid_location<int>& location) const
{
    return rooms.contains(location) && rooms.at(location) == RoomType::Spawn;
}

bool World::IsNormal(const grid_location<int>& location) const
{
    return rooms.contains(location) && rooms.at(location) == RoomType::Normal;
}

bool World::IsItem(const grid_location<int>& location) const
{
    return rooms.contains(location) && rooms.at(location) == RoomType::Item;
}

bool World::IsShop(const grid_location<int>& location) const
{
    return rooms.contains(location) && rooms.at(location) == RoomType::Shop;
}

bool World::IsSecret(const grid_location<int>& location) const
{
    return rooms.contains(location) && rooms.at(location) == RoomType::Secret;
}

bool World::IsSuperSecret(const grid_location<int>& location) const
{
    return rooms.contains(location) && rooms.at(location) == RoomType::SuperSecret;
}

bool World::IsBoss(const grid_location<int>& location) const
{
    return rooms.contains(location) && rooms.at(location) == RoomType::Boss;
}

} // namespace Dungeon