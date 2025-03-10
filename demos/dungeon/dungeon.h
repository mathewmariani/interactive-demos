#pragma once

#include <map>

#include "datastructures/grid_location.h"

namespace Dungeon
{

static constexpr int kWidth = 10;
static constexpr int kHeight = 8;

enum class RoomType : uint8_t
{
    Spawn,
    Normal,
    Item,
    Shop,
    Secret,
    Boss,
    Count,

    // always last.
    Null = 255
};

class World
{
  public:
    World();
    ~World() = default;

    void Generate(void);

    bool IsSpawn(const grid_location<int>& location) const;
    bool IsNormal(const grid_location<int>& location) const;
    bool IsItem(const grid_location<int>& location) const;
    bool IsShop(const grid_location<int>& location) const;
    bool IsSecret(const grid_location<int>& location) const;
    bool IsBoss(const grid_location<int>& location) const;

    int CountNeighbors(const grid_location<int>& location) const;

  private:
    bool Step(void);
    void Visit(const grid_location<int>& location, const RoomType type);

  private:
    std::map<grid_location<int>, RoomType> rooms;
    std::vector<grid_location<int>> layout;
    std::vector<grid_location<int>> queue;
    std::vector<grid_location<int>> endings;

    bool started = false;
    int count = 0;
    int max = 15;
    int min = 7;
};

} // namespace Dungeon