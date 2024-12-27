#include <emscripten/bind.h>

#include "grid_location.h"
#include "grid_world.h"
#include "breadthfirst_search.h"

// https://emscripten.org/docs/porting/connecting_cpp_and_javascript/embind.html
// The functions register the class, its constructor(), member function(), class_function() (static) and property().

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

    emscripten::function("BreadthFirstSearch", &BreadthFirstSearch);
}