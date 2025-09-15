#include <emscripten/bind.h>

#include "datastructures/grid_location.h"
#include "datastructures/grid_world.h"

// https://emscripten.org/docs/porting/connecting_cpp_and_javascript/embind.html
// The functions register the class, its constructor(), member function(), class_function() (static) and property().

using Location = grid_location<int>;
using LocationVector = std::vector<Location>;
using LocationMap = std::unordered_map<Location, Location>;

// mantra: I would rather write bindings in c++ than confusing javascript
EMSCRIPTEN_BINDINGS(my_module)
{
    emscripten::value_object<grid_location<int>>("GridLocation")
        .field("x", &grid_location<int>::x)
        .field("y", &grid_location<int>::y);

    emscripten::class_<grid_world>("GridWorld")
        .constructor<>()
        .constructor<int, int>()
        .property("width_readonly", &grid_world::getWidth)
        .property("height_readonly", &grid_world::getHeight)
        .function("locations", &grid_world::locations)
        .function("toggleWall", &grid_world::toggleWall)
        .function("isWall", &grid_world::isWall);

    // clang-format off
    emscripten::register_vector<Location>("LocationVector")
        .function("has", emscripten::optional_override([](LocationVector& self, const Location& loc) {
            return std::find(self.begin(), self.end(), loc) != self.end();
        }))
        .function("get", emscripten::optional_override([](LocationVector& self, const Location& loc) {
            auto it = std::find_if(self.begin(), self.end(),
                [&](const Location& l) {
                    return l == loc;
                });
            if (it != self.end())
            {
                return emscripten::val(*it); // wrap and return the GridNode
            }
            return emscripten::val::undefined(); // not found
        }));
    // clang-format on

    // clang-format off
    emscripten::class_<LocationMap>("LocationMap")
        .constructor<>()
        .function("set", emscripten::optional_override([](LocationMap& self, const Location& key, const Location& val) {
            self[key] = val;
        }))
        .function("get", emscripten::optional_override([](LocationMap& self, const Location& key) -> Location {
            auto it = self.find(key);
            if (it != self.end()) return it->second;
            return Location{ -1, -1 };
        }))
        .function("has", emscripten::optional_override([](LocationMap& self, const Location& key) {
            return self.find(key) != self.end();
        }))
        .function("keys", emscripten::optional_override([](LocationMap& self) {
            std::vector<Location> keys;
            keys.reserve(self.size());
            for (auto& kv : self) keys.push_back(kv.first);
            return keys;
        }))
        .function("values", emscripten::optional_override([](LocationMap& self) {
            std::vector<Location> vals;
            vals.reserve(self.size());
            for (auto& kv : self) vals.push_back(kv.second);
            return vals;
        }));
    // clang-format on
}