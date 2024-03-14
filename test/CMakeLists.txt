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

include(GoogleTest)
gtest_discover_tests(utils-test tuple-test matrix-test)
