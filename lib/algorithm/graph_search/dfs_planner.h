#pragma once
#include <spdlog/spdlog.h>

#include <functional>
#include <iostream>
#include <stack>

#include "data_struct.h"

namespace graph_search {

// 选择器
using FloodFillSelector = std::function<bool(const Map &, const Grid2D &)>;

// 是否满足条件
using FloodFillSearcher =
    std::function<bool(const Map &, const Grid2D &, const std::vector<uint8_t> &cost)>;

class DfsSearchPlanner {
public:
    DfsSearchPlanner() = default;
    ~DfsSearchPlanner() = default;

    void search(FloodFillSelector selector, FloodFillSearcher searcher, const Map &map,
                const Grid2D &start_point, const Grid2D &current_grid,
                const std::vector<Grid2D> &neighbours, std::vector<Grid2D> &search_area) {
        if (!selector(map, start_point)) {
            spdlog::error("start_point can not select !!!");
            return;
        }

        // DFS initialize
        int32_t count{0};
        std::stack<Grid2D> s_list;
        Grid2D neigh_point, search_point;
        ChainList visited_list;

        auto start = std::chrono::high_resolution_clock::now();
        s_list.push(start_point);
        visited_list.emplace(start_point);

        while (!s_list.empty() && count < INT32_MAX) {
            count++;
            search_point = s_list.top();
            search_area.push_back(search_point);
            s_list.pop();

            // spdlog::info("search_point[{}][{}]:{}", search_point.x, search_point.y,
            //  map.map_[search_point.x][search_point.y]);
            // for (const auto neigh : neighbours) {
            //     neigh_point.x = search_point.x + neigh.x;
            //     neigh_point.y = search_point.y + neigh.y;
            //     // spdlog::info("neigh_point[{}][{}]:{}", neigh_point.x,
            //     // neigh_point.y,
            //     //              map.map_[neigh_point.x][neigh_point.y]);
            //     if (!visited_list.count(neigh_point) && selector(map, neigh_point)) {
            //         s_list.push(neigh_point);
            //         visited_list.emplace(neigh_point);
            //     }
            // }
            bool is_select = false;
            Grid2D select_grid;

            //  1. 从8邻域中选择出候选边界点
            std::vector<Grid2D> candidate_grid;
            for (const auto neigh : neighbours) {
                neigh_point.x = search_point.x + neigh.x;
                neigh_point.y = search_point.y + neigh.y;
                if (!visited_list.count(neigh_point) && selector(map, neigh_point)) {
                    candidate_grid.push_back(neigh_point);
                }
            }

            // 2. 计算每个候选点到当前点的距离
            std::vector<std::pair<int, int>> candidate_grid_dis;
            for (int i = 0; i < candidate_grid.size(); i++) {
                int tmp_dis = current_grid.EuclideanDistance(candidate_grid.at(i));
                candidate_grid_dis.push_back(std::make_pair(i, tmp_dis));
            }

            // 3.
            int min_dis = std::numeric_limits<int>::max();
            for (int i = 0; i < candidate_grid_dis.size(); i++) {
                if (min_dis > candidate_grid_dis.at(i).second) {
                    min_dis = candidate_grid_dis.at(i).second;
                }
            }

            // 4.
            std::vector<int> candidate_index;
            for (int i = 0; i < candidate_grid_dis.size(); i++) {
                if (min_dis == candidate_grid_dis.at(i).second) {
                    candidate_index.push_back(i);
                }
            }

            // 5.
            if (candidate_index.size() == 0) {
            } else if (candidate_index.size() == 1) {
                is_select = true;
                select_grid =
                    candidate_grid.at(candidate_grid_dis.at(candidate_index.at(0)).first);
            } else {
                std::vector<std::pair<Grid2D, int>> neighbor_dis;
                for (int i = 0; i < candidate_index.size(); i++) {
                    auto tmp_grid = candidate_grid.at(
                        candidate_grid_dis.at(candidate_index.at(i)).first);
                    int tmp_distance = 0;
                    for (const auto neigh : neighbours) {
                        Grid2D tmp_neigh_grid;
                        tmp_neigh_grid.x = tmp_grid.x + neigh.x;
                        tmp_neigh_grid.y = tmp_grid.y + neigh.y;
                        if (!visited_list.count(tmp_neigh_grid) &&
                            selector(map, tmp_neigh_grid)) {
                            tmp_distance +=
                                current_grid.EuclideanDistance(tmp_neigh_grid);
                        }
                    }
                    neighbor_dis.push_back(std::make_pair(tmp_grid, tmp_distance));
                }

                int min_index = 0;
                int min_distance = std::numeric_limits<int>::max();
                for (int i = 0; i < neighbor_dis.size(); i++) {
                    if (min_distance > neighbor_dis.at(i).second) {
                        min_distance = neighbor_dis.at(i).second;
                        min_index = i;
                    }
                }
                is_select = true;
                select_grid = neighbor_dis.at(min_index).first;
            }

            if (is_select) {
                s_list.push(select_grid);
                visited_list.emplace(select_grid);
            }
        }
        // auto end = std::chrono::high_resolution_clock::now();
        // auto duration =
        //     std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
        //         .count();
        // spdlog::info("Finish DFS, count {}, cost time {} ms,size : {}", count,
        //              duration, search_area.size());
    };
};
}    // namespace graph_search
