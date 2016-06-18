cmake_minimum_required (VERSION 2.8.4)
project (Tetris)

cmake_policy(SET CMP0003 NEW)
find_package(Curses REQUIRED)

set (VERSION_MAJOR 0)
set (VERSION_MINOR 9.2)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -std=c++11 -ggdb3 -fPIC -Wno-pmf-conversions")

add_executable(tetris tetris.cc)
target_link_libraries(tetris ${CURSES_LIBRARIES})

add_executable(wartetris wartetris.cc)
target_link_libraries(wartetris ${CURSES_LIBRARIES})
