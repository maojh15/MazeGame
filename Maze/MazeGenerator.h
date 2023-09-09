//
// Created by 冒家豪 on 2023/9/8.
//

#ifndef MAZE_MAZEGENERATOR_H
#define MAZE_MAZEGENERATOR_H

#include "DisjointUnionSet.h"

#include <algorithm>
#include <vector>
#include <random>
#include <ostream>

struct MazeStruct {
    void InitializeMaze(int width, int height) {
        list_nodes.clear();
        list_nodes.resize(width * height);
    }

    struct Node {
        std::vector<int> neighbour;

        bool HasNeighbour(int neighbour_index) const {
            return std::any_of(neighbour.begin(), neighbour.end(),
                               [neighbour_index](int x) { return x == neighbour_index; });
        }

        void RemoveNeighbour(int neighbour_index) {
            for (int i = 0; i < neighbour.size(); ++i) {
                if (neighbour[i] == neighbour_index) {
                    std::swap(neighbour[i], neighbour[neighbour.size() - 1]);
                    neighbour.pop_back();
                    return;
                }
            }
        }
    };

    void AddEdge(int from, int to) {
        Node &node = list_nodes[from];
        if (node.HasNeighbour(to)) {
            return;
        }
        node.neighbour.emplace_back(to);
        list_nodes[to].neighbour.emplace_back(from);
    }

    void RemoveEdge(int from, int to) {
        list_nodes[from].RemoveNeighbour(to);
        list_nodes[to].RemoveNeighbour(from);

    }

    std::vector<Node> list_nodes;
};

class MazeGenerator {
public:

    void GenerateMaze(int width, int height, uint32_t random_seed = 0);

    const MazeStruct &GetMazeStruct() const { return maze_struct; }

    void PrintMaze(std::ostream &out) const;

private:
    DisjointUnionSet disjoint_set;
    MazeStruct maze_struct;

    int width_, height_;

    int CoordToPixelIndex(int i, int j) const {
        return i * width_ + j;
    }

    std::pair<int, int> PixelIndexToCoord(int pixel_index) const {
        return std::make_pair(pixel_index / width_, pixel_index % width_);
    }

    void RandomKnockOffWalls(uint32_t random_seed);

    void InitializeWalls(std::vector<std::pair<int, int>> &list_walls);
};


#endif //MAZE_MAZEGENERATOR_H
