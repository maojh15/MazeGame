//
// Created by 冒家豪 on 2023/9/9.
//

#include "MazeGenerator.h"

#include <string>
#include <iostream>

int main(int argc, char **argv) {
    MazeGenerator maze_generator;
    int width = 60;
    int height = 50;
    uint32_t rand_seed = 0;
    std::cout << "Please input width: ";
    std::cin >> width;
    std::cout << "Please input height: ";
    std::cin >> height;
    std::cout << "Please input random seed: ";
    std::cin >> rand_seed;
    maze_generator.GenerateMaze(width, height, rand_seed);

    maze_generator.PrintMaze(std::cout);
}