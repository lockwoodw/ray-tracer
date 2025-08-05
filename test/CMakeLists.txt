cmake_minimum_required(VERSION 3.14)
project(RayTracer)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/e4fdb87e76b9fc4b01c54ad81aea19d6e994b994.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


enable_testing()

add_executable(
  utils-test
  ../src/utils.cc
  utils.cc
)

target_link_libraries(
  utils-test
  GTest::gtest_main
)

target_include_directories(utils-test PRIVATE ../include/)

add_executable(
  tuple-test
  ../src/utils.cc
  ../src/tuple.cc
  tuple.cc
)

target_link_libraries(
  tuple-test
  GTest::gtest_main
)

target_include_directories(tuple-test PRIVATE ../include/)

add_executable(
  matrix-test
  ../src/utils.cc
  ../src/tuple.cc
  ../src/matrix.cc
  matrix.cc
)

target_link_libraries(
  matrix-test
  GTest::gtest_main
)

target_include_directories(matrix-test PRIVATE ../include/)

add_executable(
  space-test
  ../src/utils.cc
  ../src/tuple.cc
  ../src/space.cc
  space.cc
)

target_link_libraries(
  space-test
  GTest::gtest_main
)

target_include_directories(space-test PRIVATE ../include/)

add_executable(
  colour-test
  ../src/utils.cc
  ../src/tuple.cc
  ../src/colour.cc
  colour.cc
)

target_link_libraries(
  colour-test
  GTest::gtest_main
)

target_include_directories(colour-test PRIVATE ../include/)

add_executable(
  canvas-test
  ../src/utils.cc
  ../src/tuple.cc
  ../src/colour.cc
  ../src/canvas.cc
  canvas.cc
)

target_link_libraries(
  canvas-test
  GTest::gtest_main
)

target_include_directories(canvas-test PRIVATE ../include/)

add_executable(
  transformations-test
  ../src/utils.cc
  ../src/tuple.cc
  ../src/space.cc
  ../src/matrix.cc
  ../src/transformations.cc
  transformations.cc
)

target_link_libraries(
  transformations-test
  GTest::gtest_main
)

target_include_directories(transformations-test PRIVATE ../include/)

add_executable(
  ray-test
  ../src/utils.cc
  ../src/tuple.cc
  ../src/space.cc
  ../src/matrix.cc
  ../src/transformations.cc
  ray.cc
)

target_link_libraries(
  ray-test
  GTest::gtest_main
)

target_include_directories(ray-test PRIVATE ../include/)

add_executable(
  sphere-test
  ../src/utils.cc
  ../src/tuple.cc
  ../src/matrix.cc
  ../src/transformations.cc
  ../src/space.cc
  ../src/colour.cc
  ../src/material.cc
  ../src/shape.cc
  ../src/sphere.cc
  ../src/pattern.cc
  sphere.cc
)

target_link_libraries(
  sphere-test
  GTest::gtest_main
)

target_include_directories(sphere-test PRIVATE ../include/)

add_executable(
  intersections-test
  ../src/utils.cc
  ../src/tuple.cc
  ../src/matrix.cc
  ../src/transformations.cc
  ../src/space.cc
  ../src/colour.cc
  ../src/material.cc
  ../src/shape.cc
  ../src/sphere.cc
  ../src/pattern.cc
  ../src/plane.cc
  intersections.cc
)

target_link_libraries(
  intersections-test
  GTest::gtest_main
)

target_include_directories(intersections-test PRIVATE ../include/)

add_executable(
  material-test
  ../src/utils.cc
  ../src/tuple.cc
  ../src/space.cc
  ../src/colour.cc
  ../src/matrix.cc
  ../src/material.cc
  ../src/shape.cc
  ../src/sphere.cc
  ../src/pattern.cc
  material.cc
)

target_link_libraries(
  material-test
  GTest::gtest_main
)

target_include_directories(material-test PRIVATE ../include/)

add_executable(
  world-test
  ../src/utils.cc
  ../src/tuple.cc
  ../src/matrix.cc
  ../src/transformations.cc
  ../src/space.cc
  ../src/colour.cc
  ../src/material.cc
  ../src/shape.cc
  ../src/sphere.cc
  ../src/pattern.cc
  ../src/world.cc
  ../src/plane.cc
  ../src/cube.cc
  world.cc
)

target_link_libraries(
  world-test
  GTest::gtest_main
)

target_include_directories(world-test PRIVATE ../include/)

add_executable(
  camera-test
  ../src/utils.cc
  ../src/tuple.cc
  ../src/matrix.cc
  ../src/transformations.cc
  ../src/space.cc
  ../src/colour.cc
  ../src/material.cc
  ../src/shape.cc
  ../src/sphere.cc
  ../src/pattern.cc
  ../src/world.cc
  ../src/camera.cc
  ../src/canvas.cc
  camera.cc
)

target_link_libraries(
  camera-test
  GTest::gtest_main
)

target_include_directories(camera-test PRIVATE ../include/)

add_executable(
  shape-test
  ../src/utils.cc
  ../src/tuple.cc
  ../src/matrix.cc
  ../src/transformations.cc
  ../src/space.cc
  ../src/colour.cc
  ../src/material.cc
  ../src/shape.cc
  ../src/pattern.cc
  shape.cc
)

target_link_libraries(
  shape-test
  GTest::gtest_main
)

target_include_directories(shape-test PRIVATE ../include/)

add_executable(
  plane-test
  ../src/utils.cc
  ../src/tuple.cc
  ../src/matrix.cc
  ../src/space.cc
  ../src/colour.cc
  ../src/material.cc
  ../src/shape.cc
  ../src/plane.cc
  ../src/pattern.cc
  plane.cc
)

target_link_libraries(
  plane-test
  GTest::gtest_main
)

target_include_directories(plane-test PRIVATE ../include/)

add_executable(
  pattern-test
  ../src/utils.cc
  ../src/tuple.cc
  ../src/matrix.cc
  ../src/transformations.cc
  ../src/space.cc
  ../src/colour.cc
  ../src/material.cc
  ../src/shape.cc
  ../src/sphere.cc
  ../src/pattern.cc
  pattern.cc
)

target_link_libraries(
  pattern-test
  GTest::gtest_main
)

target_include_directories(pattern-test PRIVATE ../include/)

add_executable(
  cube-test
  ../src/utils.cc
  ../src/tuple.cc
  ../src/matrix.cc
  ../src/space.cc
  ../src/colour.cc
  ../src/material.cc
  ../src/shape.cc
  ../src/cube.cc
  cube.cc
)

target_link_libraries(
  cube-test
  GTest::gtest_main
)

target_include_directories(cube-test PRIVATE ../include/)

add_executable(
  sheet-test
  ../src/utils.cc
  ../src/tuple.cc
  ../src/matrix.cc
  ../src/space.cc
  ../src/colour.cc
  ../src/material.cc
  ../src/shape.cc
  ../src/sheet.cc
  sheet.cc
)

target_link_libraries(
  sheet-test
  GTest::gtest_main
)

target_include_directories(sheet-test PRIVATE ../include/)

include(GoogleTest)
gtest_discover_tests(
  utils-test
  tuple-test
  matrix-test
  space-test
  colour-test
  canvas-test
  transformations-test
  ray-test
  sphere-test
  intersections-test
  material-test
  world-test
  camera-test
  shape-test
  plane-test
  pattern-test
  cube-test
  sheet-test
)

# cmake --build build