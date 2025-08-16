#include <emscripten/bind.h>

#include "astar_search.h"
#include "breadth_first_search.h"
#include "dijkstra_search.h"
#include "greedy_search.h"

EMSCRIPTEN_BINDINGS(towerdefense_module)
{
    emscripten::value_object<std::pair<Frontier, CameFrom>>("std::pair<Frontier, CameFrom>")
        .field("first", &std::pair<Frontier, CameFrom>::first)
        .field("second", &std::pair<Frontier, CameFrom>::second);

    emscripten::function("BreadthFirstSearch", &BreadthFirstSearch);
    emscripten::function("AStarSearch", &AStarSearch);
    emscripten::function("DijkstraSearch", &DijkstraSearch);
    emscripten::function("GreedySearch", &GreedySearch);
}