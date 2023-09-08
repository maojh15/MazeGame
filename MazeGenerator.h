//
// Created by 冒家豪 on 2023/9/8.
//

#ifndef MAZE_MAZEGENERATOR_H
#define MAZE_MAZEGENERATOR_H

#include "DisjointUnionSet.h"

#include <vector>
#include <random>

class MazeGenerator {
public:
    struct WallState {
        WallState(bool left_wall_off = false, bool up_wall_off = false, bool right_wall_off = false,
                  bool down_wall_off = false)
                : is_left_wall_off(left_wall_off), is_up_wall_off(up_wall_off), is_right_wall_off(right_wall_off),
                  is_down_wall_off(down_wall_off) {}

        bool is_left_wall_off, is_up_wall_off, is_right_wall_off, is_down_wall_off;

        bool &GetWallState(int wall_index) {
            switch (wall_index) {
                case 0:
                    return is_left_wall_off;
                    break;
                case 1:
                    return is_up_wall_off;
                    break;
                case 2:
                    return is_right_wall_off;
                    break;
                case 3:
                    return is_down_wall_off;
                    break;
                default:
                    throw std::runtime_error("ERROR: Not allowed wall_index.");
            }
        }
    };

    void GenerateMaze(int width, int height, uint32_t random_seed = 0);

    const WallState &GetWallState(int i, int j) const {
        return record_wall[CoordToPixelIndex(i, j)];
    }

private:
    DisjointUnionSet disjoint_set;
    std::vector<WallState> record_wall;

    int width_, height_;

    int CoordToPixelIndex(int i, int j) const {
        return i * width_ + j;
    }

    std::pair<int, int> PixelIndexToCoord(int pixel_index) const {
        return std::make_pair(pixel_index / width_, pixel_index % width_);
    }

    void RandomKnockOffWalls(uint32_t random_seed);

    void KnockWall(int pixel_selected, int wall_selected);
};


#endif //MAZE_MAZEGENERATOR_H
