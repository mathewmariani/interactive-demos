cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(demos)

if (CMAKE_SYSTEM_NAME STREQUAL Emscripten)
    set(CMAKE_EXECUTABLE_SUFFIX ".js")

    add_executable(demos
        demos/bindings.cpp

        # chess
        demos/chess/chess.cpp
        demos/chess/wrap_chess.cpp

        # dungeon
        demos/dungeon/dungeon.cpp
        demos/dungeon/wrap_dungeon.cpp

        # minesweeper
        demos/minesweeper/minesweeper.cpp
        demos/minesweeper/wrap_minesweeper.cpp

        # tictactoe
        demos/tictactoe/tictactoe.cpp
        demos/tictactoe/wrap_tictactoe.cpp

        # pathfinding
        demos/pathfinding/astar_search.cpp
        demos/pathfinding/breadth_first_search.cpp
        demos/pathfinding/dijkstra_search.cpp
        demos/pathfinding/greedy_search.cpp
        demos/pathfinding/wrap_pathfinding.cpp

        # life
        demos/life/life.cpp
        demos/life/wrap_life.cpp
        
        # maze generator
        demos/mazegenerator/maze.cpp
        demos/mazegenerator/wrap_maze.cpp
        demos/mazegenerator/generators/huntandkill.cpp
        demos/mazegenerator/generators/randomwalk.cpp
        demos/mazegenerator/generators/recursivebacktracker.cpp)

    target_include_directories(demos PUBLIC demos)

    target_link_options(demos PRIVATE
        -lembind
        -sWASM=1
        -sEXPORT_NAME="Module"
        -sALLOW_MEMORY_GROWTH=1
        -sBINARYEN_ASYNC_COMPILATION=0
        -sDISABLE_EXCEPTION_CATCHING=0
        -sENVIRONMENT=web
        -sSINGLE_FILE=1
        -sMODULARIZE=1
        -sEXPORT_ES6=1)
endif()