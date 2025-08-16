#pragma once

#include "datastructures/grid_location.h"
#include <map>

namespace Minesweeper
{

static constexpr int kDefaultCell = 0x0000;
static constexpr int kWidth = 10;
static constexpr int kHeight = 8;
static constexpr int kMines = 10;

enum class CellType : uint8_t
{
    None = 0,            // 0b0000
    Flag = (1 << 0),     // 0b0001
    Mine = (1 << 1),     // 0b0010
    Explored = (1 << 2), // 0b0100
};

inline CellType operator~(CellType type)
{
    return static_cast<CellType>(~static_cast<std::underlying_type_t<CellType>>(type));
}

inline CellType operator&(CellType lhs, CellType rhs)
{
    return static_cast<CellType>(static_cast<uint8_t>(lhs) & static_cast<uint8_t>(rhs));
}

inline CellType operator|(CellType lhs, CellType rhs)
{
    return static_cast<CellType>(static_cast<uint8_t>(lhs) | static_cast<uint8_t>(rhs));
}

inline CellType operator^(CellType lhs, CellType rhs)
{
    return static_cast<CellType>(static_cast<uint8_t>(lhs) ^ static_cast<uint8_t>(rhs));
}

inline CellType& operator|=(CellType& lhs, CellType rhs)
{
    return lhs = lhs | rhs;
}

inline CellType& operator^=(CellType& lhs, CellType rhs)
{
    return lhs = lhs ^ rhs;
}

class Board
{
  public:
    Board();

  private:
    auto InRange(const grid_location<int>& location) const;

  public:
    void Reset();

    void ToggleFlag(const grid_location<int>& location);

    void Explore(const grid_location<int>& location);

    uint8_t GetMineCount(const grid_location<int>& location) const;

    bool IsMine(const grid_location<int>& location) const;
    bool IsExplored(const grid_location<int>& location) const;
    bool IsFlag(const grid_location<int>& location) const;

    bool CheckWin() const;

  private:
    std::map<grid_location<int>, CellType> grid;
};

} // namespace Minesweeper
