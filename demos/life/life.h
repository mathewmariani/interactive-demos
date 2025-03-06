#pragma once

#include <map>

#include "datastructures/grid_location.h"

namespace Life
{

static constexpr int kWidth = 10;
static constexpr int kHeight = 8;

enum class CellState : uint8_t
{
    Dead = 0,         // 0b0000
    Alive = (1 << 0), // 0b0001
};

class World
{
  public:
    World();
    ~World() = default;

    void Clear(void);
    void Reset(void);
    void Step(void);

    void Toggle(const grid_location<int>& location);

    bool IsAlive(const grid_location<int>& location) const;
    bool IsDead(const grid_location<int>& location) const;

    int CountNeighbors(const grid_location<int>& location) const;

  private:
    void SetNext(grid_location<int>& location, CellState state);

  private:
    int buffer_index = 0;
    std::map<grid_location<int>, CellState> buffer[2];
};

} // namespace Life