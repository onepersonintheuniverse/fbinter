cmake_minimum_required(VERSION 3.10)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

project(fbinter LANGUAGES C)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBDRM REQUIRED IMPORTED_TARGET libdrm)

add_library(fbinter STATIC src/3d.c src/colfun.c src/curve.c src/drm_plug.c src/drm_util.c src/geom.c src/global.c src/points.c src/render.c)
target_include_directories(fbinter PUBLIC include)
target_link_libraries(fbinter PUBLIC m PkgConfig::LIBDRM)

add_executable(example_cube examples/example_cube.c)
add_executable(example_param examples/example_param.c)
add_executable(example_3param examples/example_3param.c)
add_executable(example_discs examples/example_discs.c)
add_executable(example_drm examples/example_drm.c)
add_executable(example_cube_2 examples/example_cube_2.c)

target_link_libraries(example_cube PRIVATE fbinter)
target_link_libraries(example_param PRIVATE fbinter)
target_link_libraries(example_3param PRIVATE fbinter)
target_link_libraries(example_discs PRIVATE fbinter)
target_link_libraries(example_drm PRIVATE fbinter)
target_link_libraries(example_cube_2 PRIVATE fbinter)

