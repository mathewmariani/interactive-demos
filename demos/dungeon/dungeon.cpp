#include "dungeon.h"
#include <random>

namespace dungeon
{

Dungeon::Dungeon()
{
    Clear();
}

void Dungeon::Clear(void)
{
    rooms = {};
    queue = {};
    endings = {};
    started = false;
    count = 0;
    max = 15;
    min = 7;
}

void Dungeon::Generate(void)
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
        return;
    }
}

bool Dungeon::Step(void)
{
    if (!queue.empty())
    {
        auto location = queue.front();
        queue.erase(queue.begin());

        auto flag = false;
        for (const auto& dt : location.VonNewmanNeighborhood)
        {
            auto loc = dt + location;
            if (loc.x < 0 || loc.x >= kWidth || loc.y < 0 || loc.y >= kHeight)
            {
                continue;
            }

            flag = Visit(loc, RoomType::Normal) || flag;
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

bool Dungeon::Visit(const grid_location<int>& location, const RoomType type)
{
    if (rooms.contains(location))
    {
        return false;
    }

    if (CountNeighbors(location) > 1 || count >= max)
    {
        return false;
    }

    static thread_local std::mt19937 gen(std::random_device{}());
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

int Dungeon::CountNeighbors(const grid_location<int>& location) const
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

const grid_location<int> Dungeon::RandomEndRoom()
{
    if (endings.empty())
    {
        return {5, 5};
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, endings.size() - 1);

    auto idx = dist(gen);
    auto location = endings[idx];
    endings.erase(endings.begin() + idx);

    return location;
}

bool Dungeon::IsSpawn(const grid_location<int>& location) const
{
    return rooms.contains(location) && rooms.at(location) == RoomType::Spawn;
}

bool Dungeon::IsNormal(const grid_location<int>& location) const
{
    return rooms.contains(location) && rooms.at(location) == RoomType::Normal;
}

bool Dungeon::IsItem(const grid_location<int>& location) const
{
    return rooms.contains(location) && rooms.at(location) == RoomType::Item;
}

bool Dungeon::IsShop(const grid_location<int>& location) const
{
    return rooms.contains(location) && rooms.at(location) == RoomType::Shop;
}

bool Dungeon::IsSecret(const grid_location<int>& location) const
{
    return rooms.contains(location) && rooms.at(location) == RoomType::Secret;
}

bool Dungeon::IsSuperSecret(const grid_location<int>& location) const
{
    return rooms.contains(location) && rooms.at(location) == RoomType::SuperSecret;
}

bool Dungeon::IsBoss(const grid_location<int>& location) const
{
    return rooms.contains(location) && rooms.at(location) == RoomType::Boss;
}

} // namespace dungeon