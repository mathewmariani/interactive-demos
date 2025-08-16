#include <emscripten/bind.h>

#include "dungeon.h"

namespace Dungeon
{

EMSCRIPTEN_BINDINGS(dungeon_module)
{
    emscripten::class_<Dungeon::World>("DungeonGenerator")
        .constructor<>()
        .function("clear", &Dungeon::World::Clear)
        .function("generate", &Dungeon::World::Generate)
        .function("isSpawn", &Dungeon::World::IsSpawn)
        .function("isNormal", &Dungeon::World::IsNormal)
        .function("isItem", &Dungeon::World::IsItem)
        .function("isShop", &Dungeon::World::IsShop)
        .function("isSecret", &Dungeon::World::IsSecret)
        .function("isSuperSecret", &Dungeon::World::IsSuperSecret)
        .function("isBoss", &Dungeon::World::IsBoss)
        .function("countNeighbors", &Dungeon::World::CountNeighbors);
}

} // namespace Dungeon
