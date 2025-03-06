#pragma once

#include <array>

template <typename T>
struct grid_location
{
    static grid_location<T> NorthWest;
    static grid_location<T> North;
    static grid_location<T> NorthEast;
    static grid_location<T> East;
    static grid_location<T> SouthEast;
    static grid_location<T> South;
    static grid_location<T> SouthWest;
    static grid_location<T> West;

    static std::array<grid_location<T>, 8> MooresNeighborhood;
    static std::array<grid_location<T>, 4> VonNewmanNeighborhood;

    T x, y;
    grid_location() : x(0), y(0) {}
    grid_location(T x_, T y_) : x(x_), y(y_) {}
    grid_location(const grid_location&) = default;
    grid_location(grid_location&&) = default;
    grid_location<T> operator+(const grid_location<T>& rhs) const
    {
        return {x + rhs.x, y + rhs.y};
    }
    grid_location<T>& operator+=(const grid_location<T>& rhs)
    {
        x = x + rhs.x;
        y = y + rhs.y;
        return *this;
    }
    grid_location<T> operator-(const grid_location<T>& rhs) const
    {
        return {x - rhs.x, y - rhs.y};
    }
    grid_location<T>& operator-=(const grid_location<T>& rhs)
    {
        x = x - rhs.x;
        y = y - rhs.y;
        return *this;
    }
    grid_location<T>& operator=(const grid_location<T>& rhs)
    {
        x = rhs.x;
        y = rhs.y;
        return (*this);
    };
    bool operator<(const grid_location& other) const
    {
        return x < other.x || (x == other.x && y < other.y);
    }
    bool operator==(const grid_location& other) const
    {
        return (x == other.x && y == other.y);
    }
};

template <typename T>
grid_location<T> grid_location<T>::NorthWest{-1, +1};
template <typename T>
grid_location<T> grid_location<T>::North{+0, +1};
template <typename T>
grid_location<T> grid_location<T>::NorthEast{+1, +1};
template <typename T>
grid_location<T> grid_location<T>::East{+1, +0};
template <typename T>
grid_location<T> grid_location<T>::SouthEast{+1, -1};
template <typename T>
grid_location<T> grid_location<T>::South{+0, -1};
template <typename T>
grid_location<T> grid_location<T>::SouthWest{-1, -1};
template <typename T>
grid_location<T> grid_location<T>::West{-1, +0};

template <typename T>
std::array<grid_location<T>, 8> grid_location<T>::MooresNeighborhood{
    grid_location<T>::NorthWest,
    grid_location<T>::North,
    grid_location<T>::NorthEast,
    grid_location<T>::East,
    grid_location<T>::SouthEast,
    grid_location<T>::South,
    grid_location<T>::SouthWest,
    grid_location<T>::West,
};

template <typename T>
std::array<grid_location<T>, 4> grid_location<T>::VonNewmanNeighborhood{
    grid_location<T>::North,
    grid_location<T>::East,
    grid_location<T>::South,
    grid_location<T>::West,
};

template class grid_location<int>;
template class grid_location<float>;

template <>
struct std::hash<grid_location<int>>
{
    std::size_t operator()(const grid_location<int>& location) const noexcept
    {
        return std::hash<int>()(location.x ^ (location.y << 16));
    }
};