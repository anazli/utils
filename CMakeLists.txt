cmake_minimum_required(VERSION 3.15)

project(utilities LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
endif()

list(APPEND CMAKE_PREFIX_PATH "${CMAKE_BINARY_DIR}/build/${CMAKE_BUILD_TYPE}/generators")

add_library(utilities 
	include/dummy.h
	src/dummy.cpp
)

target_include_directories(utilities
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>
)

option(BUILD_TESTS "Build tests" ON)
if(BUILD_TESTS)
	enable_testing()
	add_subdirectory(test)
endif()