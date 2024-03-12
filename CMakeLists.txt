cmake_minimum_required(VERSION 3.0)

project(demo VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 11)

include_directories(inc)

file(GLOB_RECURSE SOURCES "src/*.cpp")

#set(SOURCES
#        src/Test.cpp
#        inc/Test.h
#        src/YU_MATH.cpp
#        inc/YU_MATH.h
#        inc/YU_DEFINE.h
#        src/YU_TOP.cpp
#        inc/YU_TOP.h
#        src/main.cpp
#        inc/main.h
#        src/YU_CAN.cpp
#        inc/YU_CAN.h
#        src/YU_DEBUG.cpp
#        inc/YU_DEBUG.h
#        src/YU_CAN.cpp
#)

#add_executable(demo ${SOURCES})
add_executable(demo ${SOURCES}
#        src/YU_CAN.cpp
#        src/YU_MOTOR.cpp
#        src/YU_DEBUG.cpp
#        inc/YU_DEBUG.h
#        inc/YU_MOTOR.h
#        inc/YU_CAN.h
        src/Test.cpp
        inc/Test.h
        src/YU_MATH.cpp
        inc/YU_MATH.h
        inc/YU_DEFINE.h
        inc/YU_DEFINE.h
        src/YU_TOP.cpp
        inc/YU_TOP.h
        src/YU_CONFIG.cpp
        inc/YU_CONFIG.h
        src/YU_PID.cpp
        inc/YU_PID.h
        src/YU_CHASSIS.cpp
        inc/YU_CHASSIS.h
        src/YU_UART.cpp
        inc/YU_UART.h
        src/YU_ROOT_INIT.cpp
        inc/YU_ROOT_INIT.h
        inc/YU_ROOT_INIT.h
        src/YU_THREAD.cpp
        inc/YU_THREAD.h
)

target_link_libraries(demo pthread)