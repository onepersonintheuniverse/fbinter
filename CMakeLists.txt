cmake_minimum_required(VERSION 3.10)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

project(fbinter LANGUAGES C)

include_directories(include)

add_executable(example_cube src/example_cube.c)
add_executable(example_param src/example_param.c)

target_link_libraries(example_cube PRIVATE m)
target_link_libraries(example_param PRIVATE m)
