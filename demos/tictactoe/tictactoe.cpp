#include "tictactoe.h"

namespace TicTacToe
{

Board::Board()
{
    Reset();
}

void Board::Reset()
{
    for (auto& row : board)
    {
        row.fill(EMPTY);
    }
}

int Board::HasWinner() const
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

bool Board::IsComplete() const
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

int Board::Get(int x, int y) const
{
    return board[x][y];
}

void Board::Set(int x, int y, int value)
{
    board[x][y] = value;
}

} // namespace TicTacToe