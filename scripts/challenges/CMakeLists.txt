cmake_minimum_required(VERSION 3.22)

project(RayTracerChallenges VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(
    chapter-01-projectile-csv
    ../../src/utils.cc
    ../../src/tuple.cc
    ../../src/space.cc
    chapter-01-projectile-csv.cc
)

target_include_directories(
    chapter-01-projectile-csv
    PUBLIC
    ../../include
)

add_executable(
    chapter-02-projectile-ppm
    ../../src/utils.cc
    ../../src/tuple.cc
    ../../src/space.cc
    ../../src/colour.cc
    ../../src/canvas.cc
    chapter-02-projectile-ppm.cc
)

target_include_directories(
    chapter-02-projectile-ppm
    PUBLIC
    ../../include
)

add_executable(
    chapter-04-clock
    ../../src/utils.cc
    ../../src/tuple.cc
    ../../src/space.cc
    ../../src/colour.cc
    ../../src/canvas.cc
    ../../src/matrix.cc
    ../../src/transformations.cc
    chapter-04-clock.cc
)

target_include_directories(
    chapter-04-clock
    PUBLIC
    ../../include
)

add_executable(
    chapter-05-sphere
    ../../src/utils.cc
    ../../src/tuple.cc
    ../../src/space.cc
    ../../src/colour.cc
    ../../src/canvas.cc
    ../../src/matrix.cc
    ../../src/transformations.cc
    ../../src/bounds.cc
    ../../src/material.cc
    ../../src/shape.cc
    ../../src/sphere.cc
    chapter-05-sphere.cc
)

target_include_directories(
    chapter-05-sphere
    PUBLIC
    ../../include
)

add_executable(
    chapter-06-shading
    ../../src/utils.cc
    ../../src/tuple.cc
    ../../src/space.cc
    ../../src/colour.cc
    ../../src/canvas.cc
    ../../src/matrix.cc
    ../../src/transformations.cc
    ../../src/bounds.cc
    ../../src/material.cc
    ../../src/shape.cc
    ../../src/sphere.cc
    chapter-06-shading.cc
)

target_include_directories(
    chapter-06-shading
    PUBLIC
    ../../include
)

add_executable(
    chapter-07-scene
    ../../src/utils.cc
    ../../src/tuple.cc
    ../../src/space.cc
    ../../src/colour.cc
    ../../src/canvas.cc
    ../../src/matrix.cc
    ../../src/transformations.cc
    ../../src/bounds.cc
    ../../src/material.cc
    ../../src/shape.cc
    ../../src/sphere.cc
    ../../src/camera.cc
    ../../src/world.cc
    chapter-07-scene.cc
)

target_include_directories(
    chapter-07-scene
    PUBLIC
    ../../include
)

add_executable(
    chapter-09-planes
    ../../src/utils.cc
    ../../src/tuple.cc
    ../../src/space.cc
    ../../src/colour.cc
    ../../src/canvas.cc
    ../../src/matrix.cc
    ../../src/transformations.cc
    ../../src/bounds.cc
    ../../src/material.cc
    ../../src/shape.cc
    ../../src/sphere.cc
    ../../src/plane.cc
    ../../src/camera.cc
    ../../src/world.cc
    chapter-09-planes.cc
)

target_include_directories(
    chapter-09-planes
    PUBLIC
    ../../include
)

add_executable(
    chapter-09-hexagon
    ../../src/utils.cc
    ../../src/tuple.cc
    ../../src/space.cc
    ../../src/colour.cc
    ../../src/canvas.cc
    ../../src/matrix.cc
    ../../src/transformations.cc
    ../../src/bounds.cc
    ../../src/material.cc
    ../../src/shape.cc
    ../../src/plane.cc
    ../../src/camera.cc
    ../../src/world.cc
    chapter-09-hexagon.cc
)

target_include_directories(
    chapter-09-hexagon
    PUBLIC
    ../../include
)

add_executable(
    chapter-09-submerged-blobs
    ../../src/utils.cc
    ../../src/tuple.cc
    ../../src/space.cc
    ../../src/colour.cc
    ../../src/canvas.cc
    ../../src/matrix.cc
    ../../src/transformations.cc
    ../../src/bounds.cc
    ../../src/material.cc
    ../../src/shape.cc
    ../../src/sphere.cc
    ../../src/plane.cc
    ../../src/camera.cc
    ../../src/world.cc
    chapter-09-submerged-blobs.cc
)

target_include_directories(
    chapter-09-submerged-blobs
    PUBLIC
    ../../include
)

add_executable(
    chapter-10-blended-pattern
    ../../src/utils.cc
    ../../src/tuple.cc
    ../../src/space.cc
    ../../src/colour.cc
    ../../src/canvas.cc
    ../../src/matrix.cc
    ../../src/transformations.cc
    ../../src/bounds.cc
    ../../src/material.cc
    ../../src/shape.cc
    ../../src/plane.cc
    ../../src/camera.cc
    ../../src/world.cc
    ../../src/pattern.cc
    chapter-10-blended-pattern.cc
)

target_include_directories(
    chapter-10-blended-pattern
    PUBLIC
    ../../include
)

add_executable(
    chapter-10-nested-pattern
    ../../src/utils.cc
    ../../src/tuple.cc
    ../../src/space.cc
    ../../src/colour.cc
    ../../src/canvas.cc
    ../../src/matrix.cc
    ../../src/transformations.cc
    ../../src/bounds.cc
    ../../src/material.cc
    ../../src/shape.cc
    ../../src/plane.cc
    ../../src/camera.cc
    ../../src/world.cc
    ../../src/pattern.cc
    chapter-10-nested-pattern.cc
)

target_include_directories(
    chapter-10-nested-pattern
    PUBLIC
    ../../include
)

add_executable(
    chapter-10-perturbed-pattern
    ../../src/utils.cc
    ../../src/tuple.cc
    ../../src/space.cc
    ../../src/colour.cc
    ../../src/canvas.cc
    ../../src/matrix.cc
    ../../src/transformations.cc
    ../../src/bounds.cc
    ../../src/material.cc
    ../../src/shape.cc
    ../../src/sphere.cc
    ../../src/plane.cc
    ../../src/camera.cc
    ../../src/world.cc
    ../../src/pattern.cc
    chapter-10-perturbed-pattern.cc
)

target_include_directories(
    chapter-10-perturbed-pattern
    PUBLIC
    ../../include
)

add_executable(
    chapter-11-reflections
    ../../src/utils.cc
    ../../src/tuple.cc
    ../../src/space.cc
    ../../src/colour.cc
    ../../src/canvas.cc
    ../../src/matrix.cc
    ../../src/transformations.cc
    ../../src/bounds.cc
    ../../src/material.cc
    ../../src/shape.cc
    ../../src/sphere.cc
    ../../src/plane.cc
    ../../src/camera.cc
    ../../src/world.cc
    ../../src/pattern.cc
    chapter-11-reflections.cc
)

target_include_directories(
    chapter-11-reflections
    PUBLIC
    ../../include
)

add_executable(
    chapter-11-fresnel
    ../../src/utils.cc
    ../../src/tuple.cc
    ../../src/space.cc
    ../../src/colour.cc
    ../../src/canvas.cc
    ../../src/matrix.cc
    ../../src/transformations.cc
    ../../src/bounds.cc
    ../../src/material.cc
    ../../src/shape.cc
    ../../src/sphere.cc
    ../../src/plane.cc
    ../../src/camera.cc
    ../../src/world.cc
    ../../src/pattern.cc
    chapter-11-fresnel.cc
)

target_include_directories(
    chapter-11-fresnel
    PUBLIC
    ../../include
)

add_executable(
    chapter-11-pond
    ../../src/utils.cc
    ../../src/tuple.cc
    ../../src/space.cc
    ../../src/colour.cc
    ../../src/canvas.cc
    ../../src/matrix.cc
    ../../src/transformations.cc
    ../../src/bounds.cc
    ../../src/material.cc
    ../../src/shape.cc
    ../../src/sphere.cc
    ../../src/plane.cc
    ../../src/camera.cc
    ../../src/world.cc
    ../../src/pattern.cc
    chapter-11-pond.cc
)

target_include_directories(
    chapter-11-pond
    PUBLIC
    ../../include
)
