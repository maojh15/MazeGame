//
// Created by 冒家豪 on 2023/9/8.
//

#include "MazeGenerator.h"

#include <random>

void MazeGenerator::GenerateMaze(int width, int height, uint32_t random_seed) {
    // initialize state
    width_ = width;
    height_ = height;
    int pixel_num = width * height;
    record_wall.clear();
    record_wall.resize(pixel_num);
    disjoint_set.Initialize(pixel_num);

    // leave entrance no walls.
    record_wall[0].is_up_wall_off = record_wall[0].is_left_wall_off = true;
    // knock off out walls.
    record_wall[pixel_num - 1].is_down_wall_off = record_wall[pixel_num - 1].is_right_wall_off = true;

    RandomKnockOffWalls(random_seed);
}

void MazeGenerator::RandomKnockOffWalls(uint32_t random_seed) {
    std::default_random_engine rand_engine(random_seed);
    int num_pixel = record_wall.size();
    std::uniform_int_distribution<int> rand_int(0, num_pixel - 1);
    std::uniform_int_distribution<int> rand_wall(0, 3);

    while (disjoint_set.GetNumDisjointSets() > 1) {
        int selected_pixel = rand_int(rand_engine);
        int selected_wall = rand_wall(rand_engine);
        KnockWall(selected_pixel, selected_wall);
    }
}

void MazeGenerator::KnockWall(int pixel_selected, int wall_selected) {
    auto pixel_coord = PixelIndexToCoord(pixel_selected);
    int neighbour_i, neighbour_j;
    switch (wall_selected) {
        case 0: // left
            neighbour_i = pixel_coord.first;
            neighbour_j = pixel_coord.second - 1;
            break;
        case 1: // up
            neighbour_i = pixel_coord.first - 1;
            neighbour_j = pixel_coord.second;
            break;
        case 2: // right
            neighbour_i = pixel_coord.first;
            neighbour_j = pixel_coord.second + 1;
            break;
        case 3: // down
            neighbour_i = pixel_coord.first + 1;
            neighbour_j = pixel_coord.second;
            break;
        default:
            break;
    }

    if (neighbour_i < 0 || neighbour_i >= width_ || neighbour_j < 0 || neighbour_j > height_) {
        return;
    }

    int neighbour_pixel_index = CoordToPixelIndex(neighbour_i, neighbour_j);
    bool &wall_state = record_wall[neighbour_pixel_index].GetWallState(wall_selected);
    if (!wall_state) {
        wall_state = true;
        disjoint_set.Union(pixel_selected, neighbour_pixel_index);
    }
}