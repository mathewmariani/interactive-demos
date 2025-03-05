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
    Board() { Reset(); }

  private:
    auto InRange(const grid_location<int>& location) const
    {
        return 0 <= location.x && location.x < kWidth && 0 <= location.y && location.y < kHeight;
    }

  public:
    void Reset()
    {
        for (auto y = 0; y < kHeight; ++y)
        {
            for (auto x = 0; x < kWidth; ++x)
            {
                grid[grid_location<int>{x, y}] = CellType::None;
            }
        }

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> distX(0, kWidth - 1);
        std::uniform_int_distribution<int> distY(0, kHeight - 1);

        for (auto i = 0; i < kMines; i++)
        {
            while (true)
            {
                auto location = grid_location<int>{distX(gen), distY(gen)};
                if (grid.at(location) == CellType::None)
                {
                    grid.at(location) |= CellType::Mine;
                    break;
                }
            }
        }
    }

    void ToggleFlag(const grid_location<int>& location)
    {
        grid.at(location) ^= CellType::Flag;
    }

    void Explore(const grid_location<int>& location)
    {
        if (IsFlag(location) || IsExplored(location))
        {
            return;
        }

        grid.at(location) |= CellType::Explored;

        if (!IsMine(location) && GetMineCount(location) == 0)
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

    uint8_t GetMineCount(const grid_location<int>& location) const
    {
        auto count = 0;
        for (const auto& t : location.MooresNeighborhood)
        {
            const auto n = t + location;
            if (InRange(n) && IsMine(n))
            {
                count++;
            }
        }
        return count;
    }

    bool IsMine(const grid_location<int>& location) const { return (grid.at(location) & CellType::Mine) != CellType::None; }
    bool IsExplored(const grid_location<int>& location) const { return (grid.at(location) & CellType::Explored) != CellType::None; }
    bool IsFlag(const grid_location<int>& location) const { return (grid.at(location) & CellType::Flag) != CellType::None; }

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
    std::map<grid_location<int>, CellType> grid;
};

} // namespace Minesweeper
