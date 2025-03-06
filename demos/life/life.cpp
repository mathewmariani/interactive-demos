#include "life.h"
#include <random>

namespace Life
{

World::World()
{
    Clear();
}

void World::Clear(void)
{
    for (auto i = 0; i < 2; ++i)
    {
        for (auto x = 0; x < kWidth; ++x)
        {
            for (auto y = 0; y < kHeight; ++y)
            {
                buffer[i][{x, y}] = CellState::Dead;
            }
        }
    }
}

void World::Reset(void)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, 1);

    for (auto i = 0; i < 2; ++i)
    {
        for (auto x = 0; x < kWidth; ++x)
        {
            for (auto y = 0; y < kHeight; ++y)
            {
                buffer[i][{x, y}] = static_cast<CellState>(dist(gen));
            }
        }
    }
}

void World::Toggle(const grid_location<int>& location)
{
    const auto curr = buffer[buffer_index % 2].at(location);
    buffer[buffer_index % 2].at(location) = (curr == CellState::Dead) ? CellState::Alive : CellState::Dead;
}

void World::Step(void)
{
    int n;
    bool is_alive;

    for (auto x = 0; x < kWidth; ++x)
    {
        for (auto y = 0; y < kHeight; ++y)
        {
            grid_location<int> location{x, y};
            is_alive = IsAlive(location);
            n = CountNeighbors(location);

            // Any live cell with two or three live neighbours survives.
            // Any dead cell with three live neighbours becomes a live cell.
            // All other live cells die in the next generation. Similarly, all other dead cells stay dead.
            if (is_alive && ((n == 2) || (n == 3)))
            {
                SetNext(location, CellState::Alive);
            }
            else if (!is_alive && (n == 3))
            {
                SetNext(location, CellState::Alive);
            }
            else
            {
                SetNext(location, CellState::Dead);
            }
        }
    }

    /* always last */
    buffer_index = (buffer_index + 1) % 2;
}

int World::CountNeighbors(const grid_location<int>& location) const
{
    auto count = 0;
    for (const auto& dt : location.MooresNeighborhood)
    {
        auto loc = dt + location;

        // wrapping logic along x-axis
        if (loc.x < 0)
        {
            loc.x += kWidth;
        }
        else if (loc.x >= kWidth)
        {
            loc.x %= kWidth;
        }

        // wrapping logic along y-axis
        if (loc.y < 0)
        {
            loc.y += kHeight;
        }
        else if (loc.y >= kHeight)
        {
            loc.y %= kHeight;
        }

        if (IsAlive(loc))
        {
            count++;
        }
    }
    return count;
}

bool World::IsAlive(const grid_location<int>& location) const
{
    return buffer[buffer_index % 2].at(location) == CellState::Alive;
}

bool World::IsDead(const grid_location<int>& location) const
{
    return buffer[buffer_index % 2].at(location) == CellState::Dead;
}

void World::SetNext(grid_location<int>& location, CellState state)
{
    buffer[(buffer_index + 1) % 2].at(location) = state;
}

} // namespace Life