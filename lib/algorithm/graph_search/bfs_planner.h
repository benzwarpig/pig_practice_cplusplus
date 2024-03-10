#pragma once
#include <spdlog/spdlog.h>
#include <time.h>

#include <functional>
#include <queue>
#include <unordered_set>

#include "data_struct.h"

namespace graph_search {

class BfsSearchPlanner {
public:
    BfsSearchPlanner() = default;
    ~BfsSearchPlanner() = default;

    void search(const std::function<bool(const Grid2D&, const Map&)> is_target,
                const std::function<bool(const Grid2D&, const Map&)> is_accessible,
                const Map& map, const Grid2D& start_point,
                const std::vector<Grid2D>& neighbours, ChainList& search_area) {
        if (!is_accessible(start_point, map)) {
            spdlog::info("Start point in BFS is Unaccessible!!! ({}, {})", start_point.x,
                         start_point.y);
            return;
        }

        if (is_target(start_point, map)) {
            spdlog::info("Start point is just the target, exit BFS! ({}, {})",
                         start_point.x, start_point.y);
            return;
        }

        // BFS Initialize
        int32_t count{0};
        std::queue<Grid2D> q_list;
        Grid2D neigh_point, search_point;
        ChainList visited_list;

        auto start = std::chrono::high_resolution_clock::now();
        q_list.push(start_point);
        visited_list.emplace(start_point);
        while (!q_list.empty() && count < INT32_MAX) {
            count++;
            search_point = q_list.front();
            // search_area.emplace(start_point);
            q_list.pop();

            if (is_target(search_point, map)) {
                spdlog::info("search done,exit BFS!");
                break;
            }

            for (const auto neigh : neighbours) {
                neigh_point.x = search_point.x + neigh.x;
                neigh_point.y = search_point.y + neigh.y;
                if (map.map_[neigh_point.x][neigh_point.y] == 255) {
                    search_area.emplace(neigh_point);
                }
                if (!visited_list.count(neigh_point) && is_accessible(neigh_point, map)) {
                    q_list.push(neigh_point);
                    visited_list.emplace(neigh_point);
                }
            }
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration =
            std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        spdlog::info("Finish BFS, count {}, cost time {} ms", count, duration.count());
    }
};
};    // namespace graph_search
