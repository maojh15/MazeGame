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
    maze_struct.InitializeMaze(width, height);
    disjoint_set.Initialize(pixel_num);

    RandomKnockOffWalls(random_seed);
}

void MazeGenerator::RandomKnockOffWalls(uint32_t random_seed) {
    std::default_random_engine rand_engine(random_seed);
    std::vector<std::pair<int, int>> list_walls;
    InitializeWalls(list_walls);

    int num_pixel = width_ * height_;
    while (disjoint_set.GetNumDisjointSets() > 1) {
        std::uniform_int_distribution<int> rand_int(0, list_walls.size() - 1);
        int wall_index = rand_int(rand_engine);
        const auto &wall_selected = list_walls[wall_index];
        if (disjoint_set.GetParent(wall_selected.first) == disjoint_set.GetParent(wall_selected.second)) {
            // There already exist a path from wall_selected.first to wall_selected.second.
            continue;
        }
        maze_struct.AddEdge(wall_selected.first, wall_selected.second);
        disjoint_set.Union(wall_selected.first, wall_selected.second);
        std::swap(list_walls[wall_index], list_walls[list_walls.size() - 1]);
        list_walls.pop_back();
    }
}

void MazeGenerator::InitializeWalls(std::vector<std::pair<int, int>> &list_walls) {
    for (int i = 0; i < height_ - 1; ++i) {
        for (int j = 0; j < width_ - 1; ++j) {
            list_walls.emplace_back(CoordToPixelIndex(i, j), CoordToPixelIndex(i + 1, j));
            list_walls.emplace_back(CoordToPixelIndex(i, j), CoordToPixelIndex(i, j + 1));
        }
    }
    for (int i = 0; i < height_ - 1; ++i) {
        list_walls.emplace_back(CoordToPixelIndex(i, width_ - 1), CoordToPixelIndex(i + 1, width_ - 1));
    }
    for (int j = 0; j < width_ - 1; ++j) {
        list_walls.emplace_back(CoordToPixelIndex(height_ - 1, j), CoordToPixelIndex(height_ - 1, j + 1));
    }
}

void MazeGenerator::PrintMaze(std::ostream &out) const {
    out << "num of disjoint pixels' sets: " << disjoint_set.GetNumDisjointSets() << "\n";
    out << "'rd' means exist right wall and down wall; ' d' means only down wall; 'r ' means only right wall\n";
    for (int i = 0; i < height_; ++i) {
        for (int j = 0; j < width_; ++j) {
            int pixel_index = CoordToPixelIndex(i, j);
            std::string mark;
            if (i < height_ - 1) {
                mark += maze_struct.list_nodes[pixel_index].HasNeighbour(CoordToPixelIndex(i + 1, j)) ?
                        " " : "_";
            } else {
                mark += "_";
            }
            if (j < width_ - 1) {
                mark += maze_struct.list_nodes[pixel_index].HasNeighbour(CoordToPixelIndex(i, j + 1)) ?
                        " " : "|";
            } else {
                mark += "|";
            }
            out << mark << "";
        }
        out << "\n";
    }
}