#include "minesweeper.h"
#include <random>

namespace Minesweeper
{

Board::Board()
{
    config = {
        .width = kWidth,
        .height = kHeight,
        .mines = kMines,
    };
    Reset();
}

auto Board::InRange(const grid_location<int>& location) const
{
    return 0 <= location.x && location.x < config.width && 0 <= location.y && location.y < config.height;
}

void Board::Reset()
{
    for (auto y = 0; y < config.height; ++y)
    {
        for (auto x = 0; x < config.width; ++x)
        {
            grid[grid_location<int>{x, y}] = CellType::None;
        }
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distX(0, config.width - 1);
    std::uniform_int_distribution<int> distY(0, config.height - 1);

    for (auto i = 0; i < config.mines; i++)
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

void Board::SetConfig(const Config& config)
{
    this->config = config;
    Reset();
}

void Board::ToggleFlag(const grid_location<int>& location)
{
    const auto isFlag = IsFlag(location);
    if (GetFlagCount() == 0 && !isFlag)
    {
        return;
    }

    grid.at(location) ^= CellType::Flag;
    flagCount = isFlag ? flagCount - 1 : flagCount + 1;
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

int Board::GetMineCount(const grid_location<int>& location) const
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

int Board::GetFlagCount() const
{
    return config.mines - flagCount;
}

bool Board::IsMine(const grid_location<int>& location) const { return (grid.at(location) & CellType::Mine) != CellType::None; }
bool Board::IsExplored(const grid_location<int>& location) const { return (grid.at(location) & CellType::Explored) != CellType::None; }
bool Board::IsFlag(const grid_location<int>& location) const { return (grid.at(location) & CellType::Flag) != CellType::None; }

bool Board::CheckWin() const
{
    for (auto x = 0; x < config.width; ++x)
    {
        for (auto y = 0; y < config.height; ++y)
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
