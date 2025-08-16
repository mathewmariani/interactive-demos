#include <emscripten/bind.h>

#include "life.h"

namespace Life
{

EMSCRIPTEN_BINDINGS(life_module)
{
    emscripten::class_<Life::World>("Life")
        .constructor<>()
        .function("reset", &Life::World::Reset)
        .function("clear", &Life::World::Clear)
        .function("step", &Life::World::Step)
        .function("toggle", &Life::World::Toggle)
        .function("isAlive", &Life::World::IsAlive)
        .function("isDead", &Life::World::IsDead)
        .function("countNeighbors", &Life::World::CountNeighbors);
}

} // namespace Life
