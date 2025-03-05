#include <emscripten/bind.h>

#include "datastructures/grid_location.h"
#include "datastructures/grid_world.h"

#include "towerdefense/astar_search.h"
#include "towerdefense/breadth_first_search.h"
#include "towerdefense/dijkstra_search.h"
#include "towerdefense/greedy_search.h"

#include "tictactoe/bot.h"
#include "tictactoe/tictactoe.h"

#include "minesweeper/minesweeper.h"

// https://emscripten.org/docs/porting/connecting_cpp_and_javascript/embind.html
// The functions register the class, its constructor(), member function(), class_function() (static) and property().

// mantra: I would rather write bindings in c++ than confusing javascript
EMSCRIPTEN_BINDINGS(my_module)
{
    emscripten::register_vector<grid_location<int>>("std::vector<grid_location<int>>");
    emscripten::register_map<grid_location<int>, int>("std::map<grid_location<int>, int>");
    emscripten::register_map<grid_location<int>, grid_location<int>>("std::map<grid_location<int>, grid_location<int>>");

    emscripten::value_object<std::pair<Frontier, CameFrom>>("std::pair<Frontier, CameFrom>")
        .field("first", &std::pair<Frontier, CameFrom>::first)
        .field("second", &std::pair<Frontier, CameFrom>::second);

    emscripten::class_<grid_location<int>>("GridLocation")
        .constructor<int, int>()
        .property("x", &grid_location<int>::x)
        .property("y", &grid_location<int>::y);

    emscripten::class_<grid_world>("GridWorld")
        .constructor<>()
        .constructor<int, int>()
        .property("width_readonly", &grid_world::getWidth)
        .property("height_readonly", &grid_world::getHeight)
        .function("locations", &grid_world::locations)
        .function("toggleWall", &grid_world::toggleWall)
        .function("isWall", &grid_world::isWall);

    // tower defense algorithms
    emscripten::function("BreadthFirstSearch", &BreadthFirstSearch);

    emscripten::function("AStarSearch", &AStarSearch);
    emscripten::function("DijkstraSearch", &DijkstraSearch);
    emscripten::function("GreedySearch", &GreedySearch);
}

EMSCRIPTEN_BINDINGS(TicTacToe)
{
    emscripten::class_<TicTacToe::Board>("TicTacToe")
        .constructor<>()
        .function("reset", &TicTacToe::Board::reset)
        .function("has_winner", &TicTacToe::Board::has_winner)
        .function("is_complete", &TicTacToe::Board::is_complete)
        .function("get", &TicTacToe::Board::get)
        .function("set", &TicTacToe::Board::set);

    emscripten::class_<TicTacToe::Bot>("Bot")
        .constructor<TicTacToe::Board*, int>()
        .function("random_move", &TicTacToe::Bot::random_move)
        .function("best_move", &TicTacToe::Bot::best_move);
}

EMSCRIPTEN_BINDINGS(Minesweeper)
{
    emscripten::class_<Minesweeper::Board>("Minesweeper")
        .constructor<>()
        .function("reset", &Minesweeper::Board::Reset)
        .function("getMineCount", &Minesweeper::Board::GetMineCount)
        .function("explore", &Minesweeper::Board::Explore)
        .function("flag", &Minesweeper::Board::Flag)
        .function("isExplored", &Minesweeper::Board::IsExplored)
        .function("isFlag", &Minesweeper::Board::IsFlag)
        .function("isMine", &Minesweeper::Board::IsMine)
        .function("checkWin", &Minesweeper::Board::CheckWin);
}