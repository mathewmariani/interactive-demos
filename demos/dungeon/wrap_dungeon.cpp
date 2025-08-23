#include <emscripten/bind.h>

#include "dungeon.h"

namespace dungeon
{

EMSCRIPTEN_BINDINGS(dungeon_module)
{
    emscripten::class_<Dungeon>("DungeonGenerator")
        .constructor<>()
        .function("clear", &Dungeon::Clear)
        .function("generate", &Dungeon::Generate)
        .function("isSpawn", &Dungeon::IsSpawn)
        .function("isNormal", &Dungeon::IsNormal)
        .function("isItem", &Dungeon::IsItem)
        .function("isShop", &Dungeon::IsShop)
        .function("isSecret", &Dungeon::IsSecret)
        .function("isSuperSecret", &Dungeon::IsSuperSecret)
        .function("isBoss", &Dungeon::IsBoss)
        .function("countNeighbors", &Dungeon::CountNeighbors);
}

} // namespace dungeon
