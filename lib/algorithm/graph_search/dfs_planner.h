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
using FloodFillSearcher = std::function<bool(const Map &, const Grid2D &,
                                             const std::vector<uint8_t> &cost)>;

class DfsSearchPlanner {
 public:
  DfsSearchPlanner() = default;
  ~DfsSearchPlanner() = default;

  void search(FloodFillSelector selector, FloodFillSearcher searcher,
              const Map &map, const Grid2D &start_point,
              const std::vector<Grid2D> &neighbours,
              std::vector<Grid2D> &search_area) {
    if (!selector(map, start_point)) {
      spdlog::error("start_point can not select !!!");
      return;
    }

    // DFS initialize
    int32_t count{0};
    std::stack<Grid2D> s_list;
    Grid2D neigh_point, search_point;
    std::unordered_set<Grid2D, HashPoint, EqPoint> visited_list;

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
      for (const auto neigh : neighbours) {
        neigh_point.x = search_point.x + neigh.x;
        neigh_point.y = search_point.y + neigh.y;
        // spdlog::info("neigh_point[{}][{}]:{}", neigh_point.x,
        // neigh_point.y,
        //              map.map_[neigh_point.x][neigh_point.y]);
        if (!visited_list.count(neigh_point) && selector(map, neigh_point)) {
          s_list.push(neigh_point);
          visited_list.emplace(neigh_point);
        }
      }
#if 0
      // auto is_accessible = [&neighbours](const Grid2D &g, const Grid2D &s,
      //                                    const Map &map) -> bool {
      //   // if (map.map_[g.x][g.y] == 255 || map.map_[s.x][s.y] == 255) {
      //   //   return true;
      //   // }

      //   // if (!edge_list.count(g)) return false;
      //   for (int32_t i = 0; i < neighbours.size(); i++) {
      //     int32_t n_x = s.x + neighbours.at(i).x;
      //     int32_t n_y = s.y + neighbours.at(i).y;
      //     if (n_x == g.x && n_y == g.y) {
      //       int32_t j = (i + 1) % neighbours.size();
      //       n_x = s.x + neighbours.at(j).x;
      //       n_y = s.y + neighbours.at(j).y;
      //       if (map.map_[n_x][n_y] == 255) {
      //         return true;
      //       }
      //       // if ((map[n_x][n_y] & FETCH_LOW_8_VALUE) < LETHAL_OBSTACLE) {
      //       //   return true;
      //       // }
      //       return false;
      //     }
      //   }
      //   return false;
      // };

      // // DFS Initialize
      // std::stack<size_t> s_orien_list;
      // size_t search_index{0};

      // int count{0};
      // std::stack<Grid2D> s_list;
      // Grid2D neigh_point, search_point;
      // std::unordered_set<Grid2D, HashPoint, EqPoint> visited_list;

      // size_t n_size = neighbours.size();
      // size_t rever_index = n_size / 2;

      // auto start = std::chrono::high_resolution_clock::now();
      // s_list.push(start_point);
      // s_orien_list.push(search_index);
      // visited_list.emplace(start_point);

      // for (int i = 0; i < n_size; i++) {
      //   neigh_point.x = start_point.x + neighbours.at(i).x;
      //   neigh_point.y = start_point.y + neighbours.at(i).y;
      //   if (!visited_list.count(neigh_point) &&
      //       is_accessible(neigh_point, search_point, map)) {
      //     s_list.push(neigh_point);
      //     s_orien_list.push((i + rever_index) % n_size);
      //     visited_list.emplace(neigh_point);
      //     break;
      //   }
      // }

      // std::vector<Grid2D> targets;
      // while (!s_list.empty() && count < INT32_MAX) {
      //   count++;
      //   search_point = s_list.top();
      //   search_index = s_orien_list.top();
      //   search_area.emplace(search_point);
      //   s_list.pop();
      //   s_orien_list.pop();

      //   targets.push_back(search_point);

      //   for (int i = (search_index + 1) % n_size; i != search_index;) {
      //     neigh_point.x = search_point.x + neighbours.at(i).x;
      //     neigh_point.y = search_point.y + neighbours.at(i).y;
      //     if (!visited_list.count(neigh_point) &&
      //         is_accessible(neigh_point, search_point, map)) {
      //       s_list.push(neigh_point);
      //       s_orien_list.push((i + rever_index) % n_size);
      //       visited_list.emplace(neigh_point);
      //     }
      //     i = ++i % n_size;
      //   }
      // }

      // auto end = std::chrono::high_resolution_clock::now();
      // auto duration =
      //     std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
      //         .count();
      // spdlog::info("Finish DFS, count {}, cost time {} ms, targets {}",
      // count,
      //              duration, targets.size());

#endif
    }
    // auto end = std::chrono::high_resolution_clock::now();
    // auto duration =
    //     std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
    //         .count();
    // spdlog::info("Finish DFS, count {}, cost time {} ms,size : {}", count,
    //              duration, search_area.size());
  };
};
}  // namespace graph_search
