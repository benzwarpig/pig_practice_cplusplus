#include <spdlog/spdlog.h>

#include "bfs_planner.h"
#include "data_struct.h"
#include "dfs_planner.h"

static inline std::vector<graph_search::Grid2D> RemoveIslandChain(
    const graph_search::Map& map, const graph_search::ChainList& input) {
  graph_search::DfsSearchPlanner dfs_planner;

  std::vector<std::vector<graph_search::Grid2D>> chains;
  graph_search::ChainList input_back;

  while (input_back.size() != input.size()) {
    // 1. 查找种子点
    graph_search::Grid2D seed;
    bool is_find = false;
    for (const auto& tmp : input) {
      auto grid_iter = input_back.find(tmp);
      if (grid_iter == input_back.end()) {
        seed = tmp;
        is_find = true;
        break;
      }
    }
    if (!is_find) {
      break;
    }
    // 2. 从种子开始搜索封闭链条，有序
    std::vector<graph_search::Grid2D> chain;
    auto is_accessible2 = [](const graph_search::Map& map,
                             const graph_search::Grid2D& cur_grid) {
      auto neighbor_four = graph_search::get4Neighbors();
      bool is_frontier = false;
      for (const auto neigh : neighbor_four) {
        graph_search::Grid2D tmp_grid;
        tmp_grid.x = cur_grid.x + neigh.x;
        tmp_grid.y = cur_grid.y + neigh.y;
        if (tmp_grid.x >= 0 && tmp_grid.y >= 0 && tmp_grid.x < map.kCol &&
            tmp_grid.y < map.kRow) {
          if (map.map_[tmp_grid.x][tmp_grid.y] == 0) {
            is_frontier = true;
          }
        }
      }
      bool is_free = map.map_[cur_grid.x][cur_grid.y] == 255;
      return cur_grid.x >= 0 && cur_grid.y >= 0 && cur_grid.x < map.kCol &&
             cur_grid.y < map.kRow && is_free && is_frontier;  //  && is_free
    };
    dfs_planner.search(is_accessible2, nullptr, map, seed,
                       graph_search::Map{}.getCenterGrid(),
                       graph_search::get8Neighbors(), chain);
    if (chain.size() == 0) {
      spdlog::warn("seed:[{},{}] can not accessible", seed.x, seed.y);
      input_back.emplace(seed);
    } else {
      for (const auto& tmp : chain) {
        // spdlog::info("tmp:[{},{}]", tmp.x, tmp.y);
        input_back.emplace(tmp);
      }
    }
    chains.emplace_back(std::move(chain));
  }

  std::vector<graph_search::Grid2D> ret;
  int max_index = 0;
  int tmp_size = std::numeric_limits<int>::lowest();
  spdlog::info("chains.size:{}", chains.size());
  for (int i = 0; i < chains.size(); i++) {
    if (tmp_size < chains.at(i).size()) {
      max_index = i;
      tmp_size = chains.at(i).size();
    }
    spdlog::info("i:{},chain.size:{}", i, chains.at(i).size());
    // for (int j = 0; j < chains.at(i).size(); j++) {
    //     spdlog::info("    j:{},grid[{}][{}]", j, chains.at(i).at(j).x,
    //                  chains.at(i).at(j).y);
    // }
  }
  ret = std::move(chains.at(max_index));
  return ret;
}

int main() {
  spdlog::info("enter graph_search_planner once");

  //
  // clang-format off
    uint8_t tmp_map[10][20] = {
        {255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255},
        {255,   0,   0,   0,   0,   0,   0,   0,   0,   255,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255},
        {255,   0,   0,   0,   0,   0,   0,   0,   255,   0,   255,   0,   0,   0,   0,   0,   0,   0,   0, 255},
        {255,   0,   0,   0,   0,   0,   0,  255,   0,   0,   0,   255,   0,   0,   0,   0,   0,   0,   0, 255},
        {255,   0,   0,   0,   0,   0,   0,   255,   0,   255,   0,   255,   0,   0,   0,   0,   0,   0,   0, 255},
        {255,   0,   0,   0,   0,   0,   0,   255,  0 ,   0,   0,   255, 0,   0,   0,   0,   0,   0,   0, 255},
        {255,   0,   0,   0,   0,   0,   0,   0,   255,   0,   0,   255,   0,   0,   0,   0,   0,   0,   0, 255},
        {255,   0,   0,   0,   0,   0,   0,   0,   0,   255,   255,   0,   0,   0,   0,   0,   0,   0,   0, 255},
        {255,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255},
        {255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255},
    };
  // clang-format on
  graph_search::Map map_test;
  map_test.loadMap(tmp_map);

  // 是否达到目标，默认无法到达
  auto is_target = [](const graph_search::Grid2D& cur_grid,
                      const graph_search::Map& map) { return false; };
  auto is_accessible = [&map_test](const graph_search::Grid2D& cur_grid,
                                   const graph_search::Map& map) {
    bool is_free = map_test.map_[cur_grid.x][cur_grid.y] == 0;
    return cur_grid.x >= 0 && cur_grid.y >= 0 && cur_grid.x < map_test.kCol &&
           cur_grid.y < map_test.kRow && is_free;  //  && is_free
  };

  // 构建搜索器
  graph_search::ChainList tmp_vis;
  graph_search::BfsSearchPlanner planner;
  planner.search(is_target, is_accessible, map_test, map_test.getCenterGrid(),
                 graph_search::get4Neighbors(), tmp_vis);
  spdlog::info("BFS flood search chain num:{}", tmp_vis.size());

  graph_search::DfsSearchPlanner dfs_planner;
  auto is_accessible2 = [](const graph_search::Map& map,
                           const graph_search::Grid2D& cur_grid) {
    bool is_free = map.map_[cur_grid.x][cur_grid.y] == 255;
    return cur_grid.x >= 0 && cur_grid.y >= 0 && cur_grid.x < map.kCol &&
           cur_grid.y < map.kRow && is_free;  //  && is_free
  };
  auto iter = tmp_vis.begin();
  std::vector<graph_search::Grid2D> chain;
  // 1. 泛洪搜索区域边界封闭链
  // dfs_planner.search(is_accessible2, nullptr, map_test, *iter,
  //                    graph_search::get8Neighbors(), chain);
  // spdlog::info("wxc chain size:{}", chain.size());

  // 2. 去除孤岛链，边界有序化
  auto remove_tmp = RemoveIslandChain(map_test, tmp_vis);
  spdlog::info("remove_tmp size : {}", remove_tmp.size());
}