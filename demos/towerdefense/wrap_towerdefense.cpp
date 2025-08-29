#include <emscripten/bind.h>

#include "astar_search.h"
#include "breadth_first_search.h"
#include "dijkstra_search.h"
#include "greedy_search.h"

#include "datastructures/grid_world.h"

struct BFSResult
{
    std::vector<grid_location<int>> frontier;
    std::unordered_map<grid_location<int>, grid_location<int>> came_from;
};

auto w_BreadthFirstSearch(const grid_world& world, const grid_location<int>& goal, int stepLimit)
{
    auto [frontier, came_from] = BreadthFirstSearch(world, goal, stepLimit);

    std::unordered_map<grid_location<int>, grid_location<int>> map;
    for (auto& kv : came_from)
    {
        map[kv.first] = kv.second;
    }
    return (BFSResult){
        .frontier = frontier,
        .came_from = map,
    };
}

EMSCRIPTEN_BINDINGS(towerdefense_module)
{
    emscripten::value_object<BFSResult>("BFSResult")
        .field("frontier", &BFSResult::frontier)
        .field("came_from", &BFSResult::came_from);

    emscripten::function("BreadthFirstSearch", &w_BreadthFirstSearch);
    emscripten::function("AStarSearch", &AStarSearch);
    emscripten::function("DijkstraSearch", &DijkstraSearch);
    emscripten::function("GreedySearch", &GreedySearch);
}