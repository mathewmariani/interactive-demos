#pragma once

#include <array>
#include <cstdint>
#include <iostream>
#include <random>

namespace Minesweeper
{

static constexpr int kDefaultCell = 0x0000;
static constexpr int kWidth = 10;
static constexpr int kHeight = 8;
static constexpr int kMines = 10;

// cell data is stored as a uint8_t.
// * 4-high bits stores the type
// * 4-low bits stores the mine count
using CellData = uint8_t;

enum class CellType : uint8_t
{
    Empty = 0,           // 0b0000
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
    return static_cast<CellType>(static_cast<CellData>(lhs) & static_cast<CellData>(rhs));
}

inline CellType operator|(CellType lhs, CellType rhs)
{
    return static_cast<CellType>(static_cast<CellData>(lhs) | static_cast<CellData>(rhs));
}

inline CellType operator^(CellType lhs, CellType rhs)
{
    return static_cast<CellType>(static_cast<CellData>(lhs) ^ static_cast<CellData>(rhs));
}

inline CellType& operator|=(CellType& lhs, CellType rhs)
{
    return lhs = lhs | rhs;
}

inline CellType GetType(CellData value)
{
    return static_cast<CellType>(value >> 4);
}

inline void SetType(CellData& value, CellType type)
{
    value |= (static_cast<CellData>(type) << 4);
}

inline CellData GetCount(CellData value)
{
    return value & 0x0F; // Get the lower 4 bits
}

inline void SetCount(CellData& value, CellData count)
{
    count &= 0x0F;                  // Ensure count fits in the lower 4 bits
    value = (value & 0xF0) | count; // Set the lower 4 bits
}

class Board
{
  public:
    Board() { Reset(); }

  private:
    auto InRange(const grid_location<int>& location) const
    {
        return 0 <= location.x && location.x < kWidth && 0 <= location.y && location.y < kHeight;
    }

  public:
    void Reset()
    {
        for (auto& row : data)
        {
            row.fill(kDefaultCell);
        }

        // Set up random number generation for placing mines
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> distX(0, kWidth - 1);
        std::uniform_int_distribution<int> distY(0, kHeight - 1);

        // Place mines on the board
        for (auto i = 0; i < kMines; i++)
        {
            int x, y;
            while (true)
            {
                x = distX(gen);
                y = distY(gen);
                if (GetType(data[x][y]) == CellType::Empty)
                {
                    SetType(data[x][y], CellType::Mine);
                    break;
                }
            }
        }

        // Now, count the mines around each cell
        for (auto y = 0; y < kHeight; y++)
        {
            for (auto x = 0; x < kWidth; x++)
            {
                // If the current cell is a mine, skip counting
                if (GetType(data[x][y]) == CellType::Mine)
                    continue;

                uint8_t count = 0;

                // Check all 8 neighboring cells for mines
                for (auto dy = -1; dy <= 1; ++dy)
                {
                    for (auto dx = -1; dx <= 1; ++dx)
                    {
                        // Ensure we're within bounds
                        auto nx = x + dx;
                        auto ny = y + dy;
                        if (nx >= 0 && nx < kWidth && ny >= 0 && ny < kHeight && GetType(data[nx][ny]) == CellType::Mine)
                        {
                            count++;
                        }
                    }
                }

                // Set the count of adjacent mines
                SetCount(data[x][y], count);
            }
        }
    }

    CellData Get(const grid_location<int>& location) const { return data[location.x][location.y]; }
    void Set(const grid_location<int>& location, int value) { data[location.x][location.y] = value; }

    void Flag(const grid_location<int>& location)
    {
        auto& cell = data[location.x][location.y];

        // Get the current type of the cell
        auto currentType = GetType(cell);

        // Check if the flag is already set
        if (IsFlag(location))
        {
            // Remove the flag if it's already set
            SetType(cell, static_cast<CellType>(GetType(cell) & ~CellType::Flag));
        }
        else
        {
            // Set the flag if it's not already set
            SetType(cell, static_cast<CellType>(GetType(cell) | CellType::Flag));
        }
    }
    void Explore(const grid_location<int>& location)
    {
        if (IsFlag(location) || IsExplored(location))
        {
            return;
        }
        SetType(data[location.x][location.y], CellType::Explored);

        if (IsMine(location))
        {
            return;
        }

        if (GetMineCount(location) == 0)
        {
            for (const auto& t : location.MooresNeighborhood)
            {
                const auto n = t + location;
                if (InRange(n))
                {
                    Explore(n);
                }
            }
        }
    }

    uint8_t GetMineCount(const grid_location<int>& location) const { return GetCount(data[location.x][location.y]); }

    bool IsMine(const grid_location<int>& location) const { return (GetType(data[location.x][location.y]) & CellType::Mine) != CellType::Empty; }
    bool IsExplored(const grid_location<int>& location) const { return (GetType(data[location.x][location.y]) & CellType::Explored) != CellType::Empty; }
    bool IsFlag(const grid_location<int>& location) const { return (GetType(data[location.x][location.y]) & CellType::Flag) != CellType::Empty; }

    bool CheckWin()
    {
        for (auto x = 0; x < kWidth; ++x)
        {
            for (auto y = 0; y < kHeight; ++y)
            {
                const grid_location<int>& location{x, y};
                if (IsMine(location) && !IsFlag(location))
                {
                    return false;
                }

                if (!IsMine(location) && !IsExplored(location))
                {
                    return false;
                }
            }
        }
        return true;
    }

  private:
    std::array<std::array<CellData, kHeight>, kWidth> data;
    std::map<grid_location<int>, int> grid;
};

} // namespace Minesweeper
