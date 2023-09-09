file(GLOB MazeSolverAStarSources *.cpp *.h)
add_library(MazeSolverAStar ${MazeSolverAStarSources})
target_include_directories(MazeSolverAStar INTERFACE ./)

target_link_libraries(MazeSolverAStar PRIVATE MazeGenerator)