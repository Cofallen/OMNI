cmake_minimum_required(VERSION 3.0)

project(demo VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 11)

include_directories(inc)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(demo ${SOURCES})

target_link_libraries(demo pthread)