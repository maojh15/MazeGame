cmake_minimum_required(VERSION 3.12)
project(Maze)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(Maze)

add_executable(main main.cpp)
target_include_directories(main PRIVATE Maze)
target_link_libraries(main PUBLIC MazeGenerator)

add_subdirectory(MazeGame)
add_subdirectory(MazeSolverAStar)