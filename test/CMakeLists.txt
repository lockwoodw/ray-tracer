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
  type-test
  ../src/types.cc
  types.cc
)

target_link_libraries(
  type-test
  GTest::gtest_main
)

target_include_directories(type-test PRIVATE ../include/)

include(GoogleTest)
gtest_discover_tests(type-test)
