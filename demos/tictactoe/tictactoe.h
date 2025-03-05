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
    Board() { reset(); }

    void reset()
    {
        for (auto& row : board)
        {
            row.fill(EMPTY);
        }
    }

    int has_winner() const
    {
        for (int i = 0; i < 3; ++i)
        {
            if (board[i][0] != EMPTY && board[i][0] == board[i][1] && board[i][0] == board[i][2])
            {
                return board[i][0];
            }
            if (board[0][i] != EMPTY && board[0][i] == board[1][i] && board[0][i] == board[2][i])
            {
                return board[0][i];
            }
        }
        if (board[1][1] != EMPTY &&
            ((board[0][0] == board[1][1] && board[2][2] == board[1][1]) ||
             (board[0][2] == board[1][1] && board[2][0] == board[1][1])))
        {
            return board[1][1];
        }
        return EMPTY;
    }

    bool is_complete() const
    {
        for (const auto& row : board)
        {
            for (int cell : row)
            {
                if (cell == EMPTY)
                    return false;
            }
        }
        return true;
    }

    int get(int x, int y) const { return board[x][y]; }
    void set(int x, int y, int value) { board[x][y] = value; }

  private:
    std::array<std::array<int, 3>, 3> board;
};
} // namespace TicTacToe