cmake_minimum_required(VERSION 3.20)

project(jug)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED TRUE)

set(SOURCES src/main.c)
add_executable(jug ${SOURCES})

target_include_directories(jug PRIVATE ./include/)
