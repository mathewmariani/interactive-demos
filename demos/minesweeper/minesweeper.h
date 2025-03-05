#pragma once

#include <array>
#include <cstdint>
#include <iostream>
#include <random>

namespace Minesweeper
{
static constexpr int kWidth = 10;
static constexpr int kHeight = 8;
static constexpr int kMines = 10;

enum class CellType : uint8_t
{
    Empty = 0,           // 0b0000
    Flag = (1 << 0),     // 0b0001
    Mine = (1 << 1),     // 0b0010
    Explored = (1 << 2), // 0b0100
};

inline CellType operator&(CellType lhs, CellType rhs)
{
    return static_cast<CellType>(static_cast<uint8_t>(lhs) & static_cast<uint8_t>(rhs));
}

inline CellType operator|(CellType lhs, CellType rhs)
{
    return static_cast<CellType>(static_cast<uint8_t>(lhs) | static_cast<uint8_t>(rhs));
}

inline CellType& operator|=(CellType& lhs, CellType rhs)
{
    lhs = lhs | rhs;
    return lhs;
}

inline CellType GetType(uint8_t value)
{
    return static_cast<CellType>(value >> 4);
}

inline void SetType(uint8_t& value, CellType type)
{
    value |= (static_cast<uint8_t>(type) << 4);
}

inline uint8_t GetCount(uint8_t value)
{
    return value & 0x0F; // Get the lower 4 bits
}

inline void SetCount(uint8_t& value, uint8_t count)
{
    count &= 0x0F;                  // Ensure count fits in the lower 4 bits
    value = (value & 0xF0) | count; // Set the lower 4 bits
}

class Board
{
  public:
    Board() { Reset(); }

    void Reset()
    {
        // Clear the board (0 represents empty)
        for (auto& row : data)
        {
            row.fill(0x0000);
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

    uint8_t Get(int x, int y) const { return data[x][y]; }
    void Set(int x, int y, int value) { data[x][y] = value; }

    void Flag(int x, int y) { SetType(data[x][y], CellType::Flag); }
    void Explore(int x, int y)
    {
        if (IsFlag(x, y))
        {
            return;
        }
        SetType(data[x][y], CellType::Explored);
    }

    uint8_t GetMineCount(int x, int y) const { return GetCount(data[x][y]); }

    bool IsMine(int x, int y) const { return (GetType(data[x][y]) & CellType::Mine) != CellType::Empty; }
    bool IsExplored(int x, int y) const { return (GetType(data[x][y]) & CellType::Explored) != CellType::Empty; }
    bool IsFlag(int x, int y) const { return (GetType(data[x][y]) & CellType::Flag) != CellType::Empty; }

  private:
    std::array<std::array<uint8_t, kHeight>, kWidth> data;
};
} // namespace Minesweeper
