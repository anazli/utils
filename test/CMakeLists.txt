cmake_minimum_required(VERSION 3.15)

project(utilities_tests LANGUAGES CXX)

include(FetchContent)
  FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.15.2
  )
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
  FetchContent_MakeAvailable(googletest)
  include(GoogleTest)

add_executable(utilities_test
  dummy_test.cpp
  socket_test.cpp
)

target_include_directories(utilities_test PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(utilities_test PRIVATE GTest::gtest GTest::gtest_main gmock utilities)

include(GoogleTest)
gtest_discover_tests(utilities_test)