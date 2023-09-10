//
// Created by 冒家豪 on 2023/9/9.
//

#include "MazeSolverAStar.h"

#include <algorithm>
#include <exception>
#include <vector>
#include <queue>

/**
 * Find shortest path by A*-algorithm.
 * @param maze
 * @param start_index: start point index.
 * @param end_index: end point index. If equal -1, set end point as right-down corner of maze.
 * @return a list of coordinate(i, j), return path from end point to start point.
 */
std::vector<std::pair<int, int>> MazeSolverAStar::FindShortestPath(const MazeGenerator &maze,
                                                                   int start_index,
                                                                   int end_index) {
    visited_pixel_index.clear();
    const auto &maze_struct = maze.GetMazeStruct();
    int maze_pixel_size = maze_struct.list_nodes.size();
    if (end_index == -1) {
        end_index = maze_pixel_size - 1;
    }
    auto start_coord = maze.PixelIndexToCoord(start_index);
    auto end_coord = maze.PixelIndexToCoord(end_index);

    std::vector<bool> known_index(maze_pixel_size, false);
    std::vector<int> distance(maze_pixel_size, std::numeric_limits<int>::max());
    auto comp = [&](int a, int b) {
        return distance[a] + EstimateDistanceToEndPoint(maze.PixelIndexToCoord(a), end_coord) >
               distance[b] + EstimateDistanceToEndPoint(maze.PixelIndexToCoord(b), end_coord);
    };
    std::priority_queue<int, std::vector<int>, decltype(comp)> heap(comp);
    distance[start_index] = 0;
    std::vector<int> pre_index(maze_pixel_size, -1);
    heap.emplace(start_index);
    bool find_path_flag = false;
    while (!heap.empty()) {
        int cur = heap.top();
        while (known_index[cur]) {
            heap.pop();
            if (heap.empty()) {
                throw std::runtime_error("No Path Exist!");
            }
            cur = heap.top();
        }
        if (cur == end_index) {
            find_path_flag = true;
            break;
        }
        heap.pop();
        known_index[cur] = true;
        visited_pixel_index.emplace_back(cur);
        for (int next: maze_struct.list_nodes[cur].neighbour) {
            if (known_index[next]) {
                continue;
            }
            auto next_coord = maze.PixelIndexToCoord(next);
            int new_dist = distance[cur] + 1 + EstimateDistanceToEndPoint(next_coord, end_coord);
            if (new_dist < distance[next]) {
                distance[next] = distance[cur] + 1;
                heap.push(next);
                pre_index[next] = cur;
            }
        }
    }
    if (find_path_flag) {
        int cur = end_index;
        std::vector<std::pair<int, int>> path;
        while (cur != -1) {
            path.emplace_back(maze.PixelIndexToCoord(cur));
            cur = pre_index[cur];
        }
        std::reverse(path.begin(), path.end());
        return path;
    }
    throw std::runtime_error("No Path Exist!");
}