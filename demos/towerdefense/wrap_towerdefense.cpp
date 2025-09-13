#include <emscripten/bind.h>
#include <emscripten/val.h>

#include "astar_search.h"
#include "breadth_first_search.h"
#include "dijkstra_search.h"
#include "greedy_search.h"

#include "datastructures/grid_world.h"

#include <map>
#include <queue>
#include <string>

struct BFSResult
{
    std::vector<grid_location<int>> frontier;
    std::unordered_map<grid_location<int>, grid_location<int>> came_from;
};

struct SearchResult
{
    std::vector<GridNode> frontier;
    std::unordered_map<grid_location<int>, grid_location<int>> came_from;
};

inline void throwJsError(const std::string& msg)
{
    emscripten::val err = emscripten::val::global("Error").new_(msg);
    throw err;
}

BFSResult w_BreadthFirstSearch(const grid_world& world, emscripten::val opts)
{
    grid_location<int> start{0, 0};
    grid_location<int> goal{0, 0};
    int stepLimit = 1000;

    if (opts.hasOwnProperty("start"))
    {
        emscripten::val s = opts["start"];
        start.x = s["x"].as<int>();
        start.y = s["y"].as<int>();
    }

    if (opts.hasOwnProperty("goal"))
    {
        emscripten::val g = opts["goal"];
        goal.x = g["x"].as<int>();
        goal.y = g["y"].as<int>();
    }

    if (opts.hasOwnProperty("stepLimit"))
    {
        stepLimit = opts["stepLimit"].as<int>();
    }

    auto [frontier, came_from] = BreadthFirstSearch(world, start, goal, stepLimit);

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

BFSResult w_DijkstraSearch(const grid_world& world, emscripten::val opts)
{
    grid_location<int> start{0, 0};
    grid_location<int> goal{0, 0};
    int stepLimit = 1000;

    if (opts.hasOwnProperty("start"))
    {
        emscripten::val s = opts["start"];
        start.x = s["x"].as<int>();
        start.y = s["y"].as<int>();
    }

    if (opts.hasOwnProperty("goal"))
    {
        emscripten::val g = opts["goal"];
        goal.x = g["x"].as<int>();
        goal.y = g["y"].as<int>();
    }

    if (opts.hasOwnProperty("stepLimit"))
    {
        stepLimit = opts["stepLimit"].as<int>();
    }

    auto [frontier, came_from] = DijkstraSearch(world, start, goal, stepLimit);

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

SearchResult w_AStarSearch(const grid_world& world, emscripten::val opts)
{
    grid_location<int> start{0, 0};
    grid_location<int> goal{0, 0};
    int stepLimit = 1000;

    if (opts.hasOwnProperty("start"))
    {
        emscripten::val s = opts["start"];
        start.x = s["x"].as<int>();
        start.y = s["y"].as<int>();
    }

    if (opts.hasOwnProperty("goal"))
    {
        emscripten::val g = opts["goal"];
        goal.x = g["x"].as<int>();
        goal.y = g["y"].as<int>();
    }

    if (opts.hasOwnProperty("stepLimit"))
    {
        stepLimit = opts["stepLimit"].as<int>();
    }

    auto [frontier, came_from_native] = AStarSearch(world, start, goal, stepLimit);

    std::vector<GridNode> frontierVec;
    frontierVec.reserve(frontier.size());

    while (!frontier.empty())
    {
        frontierVec.push_back(frontier.top());
        frontier.pop();
    }

    std::unordered_map<grid_location<int>, grid_location<int>> came_from;
    for (auto& kv : came_from_native)
    {
        came_from[kv.first] = kv.second;
    }

    return SearchResult{frontierVec, came_from};
}

SearchResult w_GreedySearch(const grid_world& world, emscripten::val opts)
{
    grid_location<int> start{0, 0};
    grid_location<int> goal{0, 0};
    int stepLimit = 1000;

    if (opts.hasOwnProperty("start"))
    {
        emscripten::val s = opts["start"];
        start.x = s["x"].as<int>();
        start.y = s["y"].as<int>();
    }

    if (opts.hasOwnProperty("goal"))
    {
        emscripten::val g = opts["goal"];
        goal.x = g["x"].as<int>();
        goal.y = g["y"].as<int>();
    }

    if (opts.hasOwnProperty("stepLimit"))
    {
        stepLimit = opts["stepLimit"].as<int>();
    }

    auto [frontier, came_from_native] = GreedySearch(world, start, goal, stepLimit);

    std::vector<GridNode> frontierVec;
    frontierVec.reserve(frontier.size());

    while (!frontier.empty())
    {
        frontierVec.push_back(frontier.top());
        frontier.pop();
    }

    std::unordered_map<grid_location<int>, grid_location<int>> came_from;
    for (auto& kv : came_from_native)
    {
        came_from[kv.first] = kv.second;
    }

    return SearchResult{frontierVec, came_from};
}

EMSCRIPTEN_BINDINGS(towerdefense_module)
{
    emscripten::value_object<BFSResult>("BFSResult")
        .field("frontier", &BFSResult::frontier)
        .field("came_from", &BFSResult::came_from);

    emscripten::value_object<SearchResult>("SearchResult")
        .field("frontier", &SearchResult::frontier)
        .field("came_from", &SearchResult::came_from);

    emscripten::value_object<GridNode>("GridNode")
        .field("location", &GridNode::location)
        .field("h", &GridNode::h)
        .field("g", &GridNode::g)
        .field("cost", &GridNode::cost);

    // clang-format off
    emscripten::register_vector<GridNode>("GridNodeVector")
        .function("has", emscripten::optional_override([](std::vector<GridNode>& self, const grid_location<int>& loc) {
            return std::find_if(self.begin(), self.end(), [&](const GridNode& n) {
                return n.location == loc;
            }) != self.end();
        }))
        .function("get", emscripten::optional_override([](std::vector<GridNode>& self, const grid_location<int>& loc) {
            auto it = std::find_if(self.begin(), self.end(),
                [&](const GridNode& n) {
                    return n.location == loc;
                });
            if (it != self.end())
            {
                return emscripten::val(*it); // wrap and return the GridNode
            }
            return emscripten::val::undefined(); // not found
        }));
    // clang-format on

    emscripten::function("BreadthFirstSearch", &w_BreadthFirstSearch);
    emscripten::function("AStarSearch", &w_AStarSearch);
    emscripten::function("DijkstraSearch", &w_DijkstraSearch);
    emscripten::function("GreedySearch", &w_GreedySearch);
}