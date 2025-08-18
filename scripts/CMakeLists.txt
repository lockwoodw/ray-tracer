cmake_minimum_required(VERSION 3.22)

project(RayTracerScripts VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(
    porous-sheet-example-1
    ../src/utils.cc
    ../src/tuple.cc
    ../src/space.cc
    ../src/colour.cc
    ../src/canvas.cc
    ../src/matrix.cc
    ../src/transformations.cc
    ../src/bounds.cc
    ../src/material.cc
    ../src/shape.cc
    ../src/plane.cc
    ../src/sheet.cc
    ../src/porous-sheet.cc
    ../src/group.cc
    ../src/camera.cc
    ../src/world.cc
    ../src/pattern.cc
    porous-sheet-example-1.cc
)

target_include_directories(
    porous-sheet-example-1
    PUBLIC
    ../include
)

add_executable(
    porous-sheet-example-2
    ../src/utils.cc
    ../src/tuple.cc
    ../src/space.cc
    ../src/colour.cc
    ../src/canvas.cc
    ../src/matrix.cc
    ../src/transformations.cc
    ../src/bounds.cc
    ../src/material.cc
    ../src/shape.cc
    ../src/plane.cc
    ../src/sheet.cc
    ../src/porous-sheet.cc
    ../src/group.cc
    ../src/camera.cc
    ../src/world.cc
    ../src/pattern.cc
    porous-sheet-example-2.cc
)

target_include_directories(
    porous-sheet-example-2
    PUBLIC
    ../include
)

add_executable(
    rocks
    ../src/utils.cc
    ../src/tuple.cc
    ../src/space.cc
    ../src/colour.cc
    ../src/canvas.cc
    ../src/matrix.cc
    ../src/transformations.cc
    ../src/bounds.cc
    ../src/material.cc
    ../src/shape.cc
    ../src/plane.cc
    ../src/sphere.cc
    ../src/group.cc
    ../src/sheet.cc
    ../src/camera.cc
    ../src/world.cc
    ../src/pattern.cc
    rocks.cc
)

target_include_directories(
    rocks
    PUBLIC
    ../include
)

add_executable(
    bubbles
    ../src/utils.cc
    ../src/tuple.cc
    ../src/space.cc
    ../src/colour.cc
    ../src/canvas.cc
    ../src/matrix.cc
    ../src/transformations.cc
    ../src/bounds.cc
    ../src/material.cc
    ../src/shape.cc
    ../src/plane.cc
    ../src/sphere.cc
    ../src/group.cc
    ../src/sheet.cc
    ../src/camera.cc
    ../src/world.cc
    ../src/pattern.cc
    rocks.cc
)

target_include_directories(
    bubbles
    PUBLIC
    ../include
)

target_compile_definitions(bubbles PRIVATE RAY_TRACER_ROCKS_CC_UNDERWATER)

add_executable(
    flags
    ../src/utils.cc
    ../src/tuple.cc
    ../src/space.cc
    ../src/colour.cc
    ../src/canvas.cc
    ../src/matrix.cc
    ../src/transformations.cc
    ../src/bounds.cc
    ../src/material.cc
    ../src/shape.cc
    ../src/plane.cc
    ../src/group.cc
    ../src/sheet.cc
    ../src/sphere.cc
    ../src/camera.cc
    ../src/world.cc
    ../src/pattern.cc
    flags.cc
)

target_include_directories(
    flags
    PUBLIC
    ../include
)

add_executable(
    getting-started
    ../src/utils.cc
    ../src/tuple.cc
    ../src/space.cc
    ../src/colour.cc
    ../src/canvas.cc
    ../src/matrix.cc
    ../src/transformations.cc
    ../src/bounds.cc
    ../src/material.cc
    ../src/shape.cc
    ../src/plane.cc
    ../src/group.cc
    ../src/sphere.cc
    ../src/camera.cc
    ../src/world.cc
    ../src/pattern.cc
    getting-started.cc
)

target_include_directories(
    getting-started
    PUBLIC
    ../include
)

# mkdir build
# cmake -S . -B build
# cmake --build build