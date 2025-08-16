#include <emscripten/bind.h>

#include "datastructures/grid_location.h"
#include "datastructures/grid_world.h"

// https://emscripten.org/docs/porting/connecting_cpp_and_javascript/embind.html
// The functions register the class, its constructor(), member function(), class_function() (static) and property().

// mantra: I would rather write bindings in c++ than confusing javascript
EMSCRIPTEN_BINDINGS(my_module)
{
    emscripten::register_vector<grid_location<int>>("std::vector<grid_location<int>>");
    emscripten::register_map<grid_location<int>, int>("std::map<grid_location<int>, int>");
    emscripten::register_map<grid_location<int>, grid_location<int>>("std::map<grid_location<int>, grid_location<int>>");

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
}