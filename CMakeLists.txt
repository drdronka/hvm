cmake_minimum_required(VERSION 3.20)
project(hvm CXX)

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

# This assumes the SDL source is available in vendored/SDL
add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)

# Create your game executable target as usual
add_executable(hvm WIN32 
    src/main.cpp
    src/game/game.cpp
    src/game/attr_basic.cpp
    src/game/attr_cmd.cpp
    src/game/unit_impl.cpp
    src/game/game_ctx.cpp
    src/game/gui.cpp
    src/core/list.cpp
    src/core/attr.cpp
    src/core/unit.cpp
    src/core/asset.cpp
    src/core/anim.cpp
)

target_include_directories(hvm
    PRIVATE
    inc/
    inc/common
    inc/game
    inc/core
)

# Link to the actual SDL3 library.
target_link_libraries(hvm PRIVATE 
    SDL3::SDL3
    m
)
