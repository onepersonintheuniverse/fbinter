cmake_minimum_required(VERSION 3.0)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

project(fbinter LANGUAGES C)

include_directories(include)

set(CMAKE_C_FLAGS "-lm")

add_executable(example_cube src/main.c)
