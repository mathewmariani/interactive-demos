#include "minesweeper.h"
#include <random>

namespace Minesweeper
{

Board::Board() { Reset(); }

auto Board::InRange(const grid_location<int>& location) const
{
    return 0 <= location.x && location.x < kWidth && 0 <= location.y && location.y < kHeight;
}

void Board::Reset()
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

void Board::ToggleFlag(const grid_location<int>& location)
{
    grid.at(location) ^= CellType::Flag;
}

void Board::Explore(const grid_location<int>& location)
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

uint8_t Board::GetMineCount(const grid_location<int>& location) const
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

bool Board::IsMine(const grid_location<int>& location) const { return (grid.at(location) & CellType::Mine) != CellType::None; }
bool Board::IsExplored(const grid_location<int>& location) const { return (grid.at(location) & CellType::Explored) != CellType::None; }
bool Board::IsFlag(const grid_location<int>& location) const { return (grid.at(location) & CellType::Flag) != CellType::None; }

bool Board::CheckWin() const
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

} // namespace Minesweeper
