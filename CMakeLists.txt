cmake_minimum_required(VERSION 3.16)
project(hvm)

set(CMAKE_BUILD_TYPE Debug)

# set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

# This assumes the SDL source is available in vendored/SDL
add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)

# Create your game executable target as usual
add_executable(hvm WIN32 
    src/main.c
    src/game.c
    src/list.c
    src/attr.c
    src/attr_impl.c
    src/unit.c
    src/unit_impl.c
    src/asset.c
    src/asset_def.c
)

target_include_directories(hvm
    PRIVATE
    inc/
)

# Link to the actual SDL3 library.
target_link_libraries(hvm PRIVATE SDL3::SDL3)
