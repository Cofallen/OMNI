cmake_minimum_required(VERSION 3.0)

project(demo VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 11)

include_directories(inc)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(demo ${SOURCES}
#        src/YU_CAN.cpp
#        src/YU_MOTOR.cpp
#        src/YU_DEBUG.cpp
#        inc/YU_CAN.h
#        inc/YU_DEBUG.h
#        inc/YU_MOTOR.h
#        src/YU_CAN.cpp
#        inc/YU_CAN.h
#        src/YU_CAN.cpp
        src/Test.cpp
        inc/Test.h)

target_link_libraries(demo pthread)