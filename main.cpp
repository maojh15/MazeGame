//
// Created by 冒家豪 on 2023/9/9.
//

#include "MazeGenerator.h"

#include <string>
#include <iostream>

std::string WallStateToStr(const MazeGenerator::WallState &wall_state) {
    std::string res = std::string(wall_state.is_left_wall_off ? "0" : "1") + (wall_state.is_up_wall_off ? "0" : "1")
                      + (wall_state.is_right_wall_off ? "0" : "1") + (wall_state.is_down_wall_off ? "0" : "1");
    return res;
}

int main(int argc, char **argv) {
    MazeGenerator maze_generator;
    int width = 5;
    int height = 5;
    maze_generator.GenerateMaze(width, height);

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            std::cout << WallStateToStr(maze_generator.GetWallState(i, j)) << " ";
        }
        std::cout << "\n";
    }
}