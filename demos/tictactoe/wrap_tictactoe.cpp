#include <emscripten/bind.h>

#include "bot.h"
#include "tictactoe.h"

namespace TicTacToe
{

EMSCRIPTEN_BINDINGS(tictactoe_module)
{
    emscripten::class_<TicTacToe::Board>("TicTacToe")
        .constructor<>()
        .function("reset", &TicTacToe::Board::Reset)
        .function("has_winner", &TicTacToe::Board::HasWinner)
        .function("is_complete", &TicTacToe::Board::IsComplete)
        .function("get", &TicTacToe::Board::Get)
        .function("set", &TicTacToe::Board::Set);

    emscripten::class_<TicTacToe::Bot>("Bot")
        .constructor<TicTacToe::Board*, int>()
        .function("random_move", &TicTacToe::Bot::RandomMove)
        .function("best_move", &TicTacToe::Bot::BestMove);
}

} // namespace TicTacToe
