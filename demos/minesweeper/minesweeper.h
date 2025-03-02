#pragma once

#include <array>

namespace Minesweeper
{
  static constexpr int kWidth = 10;
  static constexpr int kHeight = 8;
  static constexpr int kMines = 10;

  enum class Value : int
  {
    Unknown = 0, // still unchecked
    Flag = 1,    // theres a flag here
    Empty = 2,   // explored
    Mine = 3,    // well... you know
  };

  // colors:
  // #AAD751
  // #A2D149

  class Board
  {
  public:
    Board() { Reset(); }

    void Reset()
    {
      for (auto &row : data)
      {
        row.fill((int)Value::Unknown);
      }

      for (auto &row : field)
      {
        row.fill((int)Value::Unknown);
      }
    }

    int Get(int x, int y) const { return data[x][y]; }
    void Set(int x, int y, int value) { data[x][y] = value; }

  private:
    bool GetMineCount(const int x, const int y) const
    {
      auto count = 0;
      if (Get(x - 1, y + 1) == (int)Value::Mine)
        count++;
      if (Get(x, y + 1) == (int)Value::Mine)
        count++;
      if (Get(x + 1, y + 1) == (int)Value::Mine)
        count++;
      if (Get(x - 1, y) == (int)Value::Mine)
        count++;
      if (Get(x + 1, y) == (int)Value::Mine)
        count++;
      if (Get(x - 1, y - 1) == (int)Value::Mine)
        count++;
      if (Get(x, y - 1) == (int)Value::Mine)
        count++;
      if (Get(x + 1, y - 1) == (int)Value::Mine)
        count++;
      return count;
    }

  private:
    std::array<std::array<int, kWidth>, kHeight> field; // visuals
    std::array<std::array<int, kWidth>, kHeight> data;  // underlying data
  };
}