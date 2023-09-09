file(GLOB Maze_Sources *.h *.cpp)
add_library(MazeGenerator SHARED ${Maze_Sources})
target_include_directories(MazeGenerator INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})