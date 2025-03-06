cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(demos)

if (CMAKE_SYSTEM_NAME STREQUAL Emscripten)
    set(CMAKE_EXECUTABLE_SUFFIX ".js")

    add_executable(demos demos/bindings.cpp)
    target_include_directories(demos PUBLIC demos)

    target_link_options(demos PRIVATE
        -lembind
        -sWASM=1
        -sALLOW_MEMORY_GROWTH=1
        -sBINARYEN_ASYNC_COMPILATION=0
        -sENVIRONMENT=web
        -sSINGLE_FILE=1
        -sMODULARIZE=1
        -sEXPORT_ES6=1)
endif()