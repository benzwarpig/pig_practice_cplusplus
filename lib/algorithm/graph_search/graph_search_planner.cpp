#include "graph_search_planner.h"

using namespace graph_search;

int main() {
    spdlog::info("enter graph_search_planner once");

    // 构建地图并载入
    // clang-format off
    uint8_t tmp_map[10][20] = {
        {255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255},
        {255,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255},
        {255,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255},
        {255,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255},
        {255,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255},
        {255,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   255, 0,   0,   0,   0,   0,   0,   0, 255},
        {255,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255},
        {255,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255},
        {255,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255},
        {255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255},
    };
    // clang-format on
    Map map_test;
    map_test.loadMap(tmp_map);

    // 是否达到目标，默认无法到达
    auto is_target = [](const Grid2D& cur_grid, const Map& map) { return false; };
    auto is_accessible = [&map_test](const Grid2D& cur_grid, const Map& map) {
        bool is_free = map_test.map_[cur_grid.x][cur_grid.y] < 255;
        return cur_grid.x >= 0 && cur_grid.y >= 0 && cur_grid.x < map_test.kCol &&
               cur_grid.y < map_test.kRow && is_free;
    };

    // 构建搜索器
    std::unordered_set<Grid2D, HashPoint, EqPoint> tmp_vis;
    BfsSearchPlanner planner;
    planner.search(is_target, is_accessible, map_test, map_test.getCenterGrid(),
                   get8Neighbors(), tmp_vis);

    spdlog::info("area edge is {}", tmp_vis.size());
    return 0;
}