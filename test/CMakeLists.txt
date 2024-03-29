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
  ../src/space.cc
  ../src/sphere.cc
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
  ../src/space.cc
  ../src/sphere.cc
  intersections.cc
)

target_link_libraries(
  intersections-test
  GTest::gtest_main
)

target_include_directories(intersections-test PRIVATE ../include/)

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
)
