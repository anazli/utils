cmake_minimum_required(VERSION 3.15)

project(utilities LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
endif()

list(APPEND CMAKE_PREFIX_PATH "${CMAKE_BINARY_DIR}/build/${CMAKE_BUILD_TYPE}/generators")

include(FetchContent)

FetchContent_Declare(
	sqlite3
	GIT_REPOSITORY https://github.com/sqlite/sqlite.git
	GIT_TAG        master
)
FetchContent_MakeAvailable(sqlite3)

add_library(utilities
	include/networking/socket.h
	src/networking/socket.cpp
	include/networking/tcp_client.h
	src/networking/tcp_client.cpp
	include/networking/tcp_server.h
	src/networking/tcp_server.cpp
	include/networking/udp_client.h
	src/networking/udp_client.cpp
	include/networking/udp_server.h
	src/networking/udp_server.cpp
)

target_include_directories(utilities
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>
)

add_executable(server src/main/server.cpp)
target_include_directories(server PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
target_link_libraries(server PRIVATE utilities)

add_executable(client src/main/client.cpp)
target_include_directories(client PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
target_link_libraries(client PRIVATE utilities)

add_executable(sqlite src/main/sqlite.cpp)
target_include_directories(sqlite PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
target_link_libraries(sqlite PRIVATE utilities sqlite3)

if(ENABLE_TESTING)
	enable_testing()
	add_subdirectory(test)
endif()