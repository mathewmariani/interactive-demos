#pragma once

#include "tictactoe.h"
#include <limits>
#include <optional>
#include <random>

namespace TicTacToe
{
class Bot
{
  public:
    explicit Bot(Board* board, int symbol) : board(board), symbol(symbol) {}

    void random_move()
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<double> dist(0.0, 1.0);

        for (int i = 0; i < 3; ++i)
        {
            for (int j = 0; j < 3; ++j)
            {
                if (board->get(i, j) == TicTacToe::EMPTY && dist(gen) > 0.6)
                {
                    board->set(i, j, symbol);
                    return;
                }
            }
        }
    }

    int minimax(bool is_maximizing, int depth)
    {
        auto who = board->has_winner();
        if (who != TicTacToe::EMPTY || board->is_complete())
        {
            return scores[who];
        }

        auto best_score = is_maximizing ? std::numeric_limits<int>::min() : std::numeric_limits<int>::max();

        for (auto i = 0; i < 3; ++i)
        {
            for (auto j = 0; j < 3; ++j)
            {
                if (board->get(i, j) == TicTacToe::EMPTY)
                {
                    board->set(i, j, is_maximizing ? TicTacToe::CROSSES : TicTacToe::NOUGHTS);
                    auto score = minimax(!is_maximizing, depth + 1);
                    best_score = is_maximizing ? std::max(score - depth, best_score) : std::min(score + depth, best_score);
                    board->set(i, j, TicTacToe::EMPTY);
                }
            }
        }
        return best_score;
    }

    void best_move()
    {
        auto is_maximizing = (symbol == TicTacToe::CROSSES);
        auto best_score = is_maximizing ? std::numeric_limits<int>::min() : std::numeric_limits<int>::max();
        std::optional<std::pair<int, int>> best_position;

        for (auto i = 0; i < 3; ++i)
        {
            for (auto j = 0; j < 3; ++j)
            {
                if (board->get(i, j) == TicTacToe::EMPTY)
                {
                    board->set(i, j, symbol);
                    auto score = minimax(!is_maximizing, 0);
                    board->set(i, j, TicTacToe::EMPTY);

                    if ((is_maximizing && score > best_score) || (!is_maximizing && score < best_score))
                    {
                        best_score = score;
                        best_position = {i, j};
                    }
                }
            }
        }
        if (best_position)
        {
            board->set(best_position->first, best_position->second, symbol);
        }
    }

  private:
    Board* board;
    int symbol;
    const int scores[3] = {0, 20, -20};
};
} // namespace TicTacToe