cmake_minimum_required(VERSION 3.12)
project(Maze)

set(CMAKE_CXX_STANDARD 14)

set(src_root "src")
add_subdirectory(${src_root}/Maze)

add_executable(test test.cpp)
target_include_directories(test PRIVATE Maze)
target_link_libraries(test PUBLIC MazeGenerator)

add_subdirectory(${src_root}/MazeGame)
add_subdirectory(${src_root}/MazeSolverAStar)
