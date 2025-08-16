#pragma once

#include <array>
#include <tuple>

namespace TicTacToe
{

static constexpr int EMPTY = 0;
static constexpr int CROSSES = 1;
static constexpr int NOUGHTS = 2;

class Board
{
  public:
    Board();

    void Reset();

    int HasWinner() const;

    bool IsComplete() const;

    int Get(int x, int y) const;
    void Set(int x, int y, int value);

  private:
    std::array<std::array<int, 3>, 3> board;
};
} // namespace TicTacToe