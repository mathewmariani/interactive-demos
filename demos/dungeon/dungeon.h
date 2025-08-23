#pragma once

#include <map>

#include "datastructures/grid_location.h"

namespace dungeon
{

static constexpr int kWidth = 11;
static constexpr int kHeight = 11;

enum class RoomType : uint8_t
{
    Spawn,
    Normal,
    Item,
    Shop,
    Secret,
    SuperSecret,
    Boss,
    Count,

    // always last.
    Null = 255
};

class Dungeon
{
  public:
    Dungeon();
    ~Dungeon() = default;

    void Clear(void);
    void Generate(void);

    bool IsSpawn(const grid_location<int>& location) const;
    bool IsNormal(const grid_location<int>& location) const;
    bool IsItem(const grid_location<int>& location) const;
    bool IsShop(const grid_location<int>& location) const;
    bool IsSecret(const grid_location<int>& location) const;
    bool IsSuperSecret(const grid_location<int>& location) const;
    bool IsBoss(const grid_location<int>& location) const;

    int CountNeighbors(const grid_location<int>& location) const;

  private:
    bool Step(void);
    bool Visit(const grid_location<int>& location, const RoomType type);

    const grid_location<int> RandomEndRoom();

  private:
    std::map<grid_location<int>, RoomType> rooms;
    std::vector<grid_location<int>> queue;
    std::vector<grid_location<int>> endings;

    bool started = false;
    int count = 0;
    int max = 15;
    int min = 7;
};

} // namespace dungeon