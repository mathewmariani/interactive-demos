#include <emscripten/bind.h>

#include "minesweeper.h"

namespace Minesweeper
{

EMSCRIPTEN_BINDINGS(minesweeper_module)
{
    emscripten::value_object<Config>("Config")
        .field("width", &Config::width)
        .field("height", &Config::height)
        .field("mines", &Config::mines);

    emscripten::class_<Minesweeper::Board>("Minesweeper")
        .constructor<>()
        .function("reset", &Minesweeper::Board::Reset)
        .function("setConfig", &Minesweeper::Board::SetConfig)
        .function("getMineCount", &Minesweeper::Board::GetMineCount)
        .function("getFlagCount", &Minesweeper::Board::GetFlagCount)
        .function("explore", &Minesweeper::Board::Explore)
        .function("toggleFlag", &Minesweeper::Board::ToggleFlag)
        .function("isExplored", &Minesweeper::Board::IsExplored)
        .function("isFlag", &Minesweeper::Board::IsFlag)
        .function("isMine", &Minesweeper::Board::IsMine)
        .function("checkWin", &Minesweeper::Board::CheckWin);
}

} // namespace Minesweeper
