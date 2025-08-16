#include <emscripten/bind.h>

#include "minesweeper.h"

namespace Minesweeper
{

EMSCRIPTEN_BINDINGS(minesweeper_module)
{
    emscripten::class_<Minesweeper::Board>("Minesweeper")
        .constructor<>()
        .function("reset", &Minesweeper::Board::Reset)
        .function("getMineCount", &Minesweeper::Board::GetMineCount)
        .function("explore", &Minesweeper::Board::Explore)
        .function("toggleFlag", &Minesweeper::Board::ToggleFlag)
        .function("isExplored", &Minesweeper::Board::IsExplored)
        .function("isFlag", &Minesweeper::Board::IsFlag)
        .function("isMine", &Minesweeper::Board::IsMine)
        .function("checkWin", &Minesweeper::Board::CheckWin);
}

} // namespace Minesweeper
