#include <emscripten/bind.h>

#include "maze.h"

#include "mazegenerator/generators/huntandkill.h"
#include "mazegenerator/generators/randomwalk.h"
#include "mazegenerator/generators/recursizebacktracker.h"

namespace maze
{

EMSCRIPTEN_BINDINGS(maze_module)
{
    emscripten::enum_<NodeType>("NodeType")
        .value("Unvisited", NodeType::Unvisited)
        .value("Frontier", NodeType::Frontier)
        .value("Visited", NodeType::Visited);

    emscripten::value_object<MazeNode>("MazeNode")
        .field("north_edge", &MazeNode::north_edge)
        .field("west_edge", &MazeNode::west_edge)
        .field("type", &MazeNode::type);

    emscripten::class_<HuntAndKill>("HuntAndKill")
        .constructor<>()
        .function("name", &HuntAndKill::GetName)
        .function("step", &HuntAndKill::Step)
        .function("clear", &HuntAndKill::Clear);

    emscripten::class_<RandomWalk>("RandomWalk")
        .constructor<>()
        .function("name", &RandomWalk::GetName)
        .function("step", &RandomWalk::Step)
        .function("clear", &RandomWalk::Clear);

    emscripten::class_<RecursiveBacktracker>("RecursiveBacktracker")
        .constructor<>()
        .function("name", &RecursiveBacktracker::GetName)
        .function("step", &RecursiveBacktracker::Step)
        .function("clear", &RecursiveBacktracker::Clear);

    emscripten::class_<Maze>("Maze")
        .constructor<>()
        .function("node", &Maze::GetNode)
        .function("resize", &Maze::Resize)

        // .function("getNodeType", &Maze::GetNodeType)
        // .function("getNodeType", &Maze::SetNodeType)

        .function("getNorth", &Maze::GetNorth)
        .function("getEast", &Maze::GetEast)
        .function("getSouth", &Maze::GetSouth)
        .function("getWest", &Maze::GetWest)

        .function("setNorth", &Maze::SetNorth)
        .function("setEast", &Maze::SetEast)
        .function("setSouth", &Maze::SetSouth)
        .function("setWest", &Maze::SetWest)

        .function("size", &Maze::Size)
        .function("width", &Maze::Width);
}

} // namespace maze
