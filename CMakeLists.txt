cmake_minimum_required(VERSION 3.0)

project(demo VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 11)
#set(BUILD_TYPE "debug" CACHE STRING "Choose the type of build(debug/release)")
#set(CMAKE_BUILD_TYPE release)
message(Default Mode: release)

include_directories(inc)

file(GLOB_RECURSE SOURCES "src/*.cpp")

if (CMAKE_BUILD_TYPE STREQUAL "debug")
    set(CMAKE_BUILD_TYPE Debug)
    add_definitions(-DBUILD_DEBUG)
elseif (CMAKE_BUILD_TYPE STREQUAL "release")
    set(CMAKE_BUILD_TYPE Release)
    add_definitions(-DBUILD_RELEASE)
elseif ()
    set(CMAKE_BUILD_TYPE Release)
    add_definitions(-DBUILD_RELEASE)
endif()

add_executable(demo ${SOURCES})

target_link_libraries(demo pthread)