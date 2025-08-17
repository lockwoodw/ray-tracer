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

# mkdir build
# cmake -S . -B build
# cmake --build build