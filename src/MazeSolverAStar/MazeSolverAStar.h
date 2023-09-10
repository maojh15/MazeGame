//
// Created by 冒家豪 on 2023/9/9.
//

#ifndef MAZE_MAZESOLVERASTAR_H
#define MAZE_MAZESOLVERASTAR_H

#include "MazeGenerator.h"

class MazeSolverAStar {
public:
    std::vector<std::pair<int, int>>
    FindShortestPath(const MazeGenerator &maze, int start_index = 0, int end_index = -1);

    std::vector<int> visited_pixel_index;
private:
    static int EstimateDistanceToEndPoint(const std::pair<int, int> &cur_point,
                                          const std::pair<int, int> &end_point) {
        // Manhattan distance.
        return std::abs(end_point.first - cur_point.first) + std::abs(end_point.second - cur_point.second);
    }

};


#endif //MAZE_MAZESOLVERASTAR_H
