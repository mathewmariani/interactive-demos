cmake_minimum_required(VERSION 3.5)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(gpr340)

if (CMAKE_SYSTEM_NAME STREQUAL Emscripten)
    set(CMAKE_EXECUTABLE_SUFFIX ".js")

    add_executable(gpr340 assignments/bindings.cpp)
    target_include_directories(gpr340 PUBLIC assignments)

    target_link_options(gpr340 PRIVATE
        -lembind
        -sWASM=1
        -sALLOW_MEMORY_GROWTH=1
        -sBINARYEN_ASYNC_COMPILATION=0
        -sSINGLE_FILE=1
        -sMODULARIZE=1
        -sEXPORT_ES6=1
    )

    add_custom_command(
        TARGET gpr340 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
            $<TARGET_FILE:gpr340>
            ${CMAKE_CURRENT_SOURCE_DIR}/../website/assets/js
    )
endif()
