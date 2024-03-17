#pragma once
#include <spdlog/spdlog.h>

#include <array>
#include <iostream>
#include <vector>

namespace graph_search {

/** 定义栅格结构 */
struct Grid2D {
public:
    int32_t x{0};
    int32_t y{0};
    float yaw{0.0};

    Grid2D() = default;
    Grid2D(int32_t x_, int32_t y_) : x(x_), y(y_) {}

    Grid2D(int32_t x_, int32_t y_, float yaw_) : x(x_), y(y_), yaw(yaw_) {}
    Grid2D(const Grid2D &grid) {
        x = grid.x;
        y = grid.y;
        yaw = grid.yaw;
    }
    Grid2D operator=(const Grid2D &grid) {
        x = grid.x;
        y = grid.y;
        yaw = grid.yaw;

        return *this;
    }
    // 目前自定义操作符和hash函数仅考虑x,y.不考虑yaw.
    bool operator<(const Grid2D &rhs) const {
        if (x == rhs.x) {
            return y < rhs.y;
        }
        return x < rhs.x;
    }

    bool operator==(const Grid2D &rhs) const { return (x == rhs.x) && (y == rhs.y); }

    struct HashFunction {
        size_t operator()(const Grid2D &rhs) const {
            size_t xHash = std::hash<int32_t>()(rhs.x);
            size_t yHash = std::hash<int32_t>()(rhs.y) << 1;
            return xHash ^ yHash;
        }
    };

    void show() const {
        std::cout << "x: " << this->x << "  "
                  << "y: " << this->y << "  "
                  << "yaw: " << this->yaw << std::endl;
    }

    inline double LackSqrtDistanceTo(const Grid2D &to) const {
        auto dx = to.x - x;
        auto dy = to.y - y;
        return dx * dx + dy * dy;
    }

    inline int EuclideanDistance(const Grid2D &to) const {
        return std::abs(to.x - this->x) + std::abs(to.y - this->y);
    }

    ~Grid2D() = default;
};

namespace details {
struct HashPoint {
    size_t operator()(const Grid2D &p) const { return (p.x + p.y * INT32_MAX); }
};

struct EqPoint {
    bool operator()(const Grid2D &p1, const Grid2D &p2) const {
        return (p1.x == p2.x && p1.y == p2.y);
    }
};
}    // namespace details

using ChainList = std::unordered_set<Grid2D, details::HashPoint, details::EqPoint>;

static std::vector<Grid2D> get4Neighbors(int step = 1) {
    return std::vector<Grid2D>{Grid2D(step, 0), Grid2D(0, step), Grid2D(-step, 0),
                               Grid2D(0, -step)};
}

static std::vector<Grid2D> get8Neighbors(int step = 1) {
    return std::vector<Grid2D>{
        Grid2D(step, 0),  Grid2D(step, step),   Grid2D(0, step),  Grid2D(-step, step),
        Grid2D(-step, 0), Grid2D(-step, -step), Grid2D(0, -step), Grid2D(step, -step)};
}

std::vector<Grid2D> GetAntiClockWiseGrids() {
    return std::vector<Grid2D>{Grid2D(1, 0),  Grid2D(1, 1),  Grid2D(0, 1),
                               Grid2D(-1, 1), Grid2D(-1, 0), Grid2D(-1, -1),
                               Grid2D(0, -1), Grid2D(1, -1)};
};

std::vector<Grid2D> GetClockWiseGrids() {
    return std::vector<Grid2D>{Grid2D(1, 0),   Grid2D(1, -1), Grid2D(0, -1),
                               Grid2D(-1, -1), Grid2D(-1, 0), Grid2D(-1, 1),
                               Grid2D(0, 1),   Grid2D(1, 1)};
};

std::vector<Grid2D> GetCrossedGrids(const int32_t &step) {
    return std::vector<Grid2D>{Grid2D(step, 0), Grid2D(0, step), Grid2D(-step, 0),
                               Grid2D(0, -step)};
};

struct Map {
    const int kCol = 10;
    const int kRow = 20;
    uint8_t map_[10][20];

    inline Grid2D getCenterGrid() { return {kCol / 2, kRow / 2}; }

    void loadMap(uint8_t tmp[10][20]) {
        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 20; j++) {
                map_[i][j] = tmp[i][j];
            }
        }
    }
};

// bool FootprintHandle::isInside(double x, double y,
//                                const std::vector<geometry::MpbPose>
//                                &top_footprint) {
//     auto polygon_ = top_footprint;
//     // 射线法判断点是否在多边形内部
//     int i, j;
//     bool c = false;
//     for (i = 0, j = polygon_.size() - 1; i < polygon_.size(); j = i++) {
//         if (((polygon_[i].y > y) != (polygon_[j].y > y)) &&
//             (x < (polygon_[j].x - polygon_[i].x) * (y - polygon_[i].y) /
//                          (polygon_[j].y - polygon_[i].y) +
//                      polygon_[i].x)) {
//             c = !c;
//         }
//     }
//     return c;
// }

// std::vector<geometry::MpbGrid2D> FootprintHandle::topGrid2FootprintGrids(
//     const std::vector<geometry::MpbGrid2D> &top_grid,
//     const std::vector<geometry::MpbPose> &top_footprint) {
//     if (top_grid.size() < 3) {
//         TRACE_WARN("top grids is too less !!!");
//         return {};
//     }
//     std::vector<geometry::MpbGrid2D> ret;
//     int min_x = std::min({top_grid.at(0).x, top_grid.at(1).x,
//     top_grid.at(2).x, top_grid.at(3).x}); int max_x =
//     std::max({top_grid.at(0).x, top_grid.at(1).x, top_grid.at(2).x,
//     top_grid.at(3).x}); int min_y = std::min({top_grid.at(0).y,
//     top_grid.at(1).y, top_grid.at(2).y, top_grid.at(3).y}); int max_y =
//     std::max({top_grid.at(0).y, top_grid.at(1).y, top_grid.at(2).y,
//     top_grid.at(3).y});

//     auto checkOutLimit = [](int tmp) {
//         tmp = std::abs(tmp);
//         if (tmp < 0 || tmp >= 9999) {
//             return false;
//         }
//         return true;
//     };
//     if (!checkOutLimit(min_x) || !checkOutLimit(max_x) ||
//     !checkOutLimit(min_y) ||
//         !checkOutLimit(max_y)) {
//         //
//         TRACE_WARN("足迹超出边界,min_x:{%d},max_x:{%d},min_y:{%d},max_y:{%d}!!!!",
//         // min_x, max_x,
//         //            min_y, max_y);
//         return {};
//     }

//     for (int i = min_x; i <= max_x; i++) {
//         for (int j = min_y; j <= max_y; j++) {
//             geometry::MpbPose pose = transform::MpbMapToWorld({i, j});

//             if (isInside(pose.x, pose.y, top_footprint)) {
//                 geometry::MpbGrid2D tmpGrid;
//                 tmpGrid = transform::MpbWorldToMap(pose);

//                 ret.push_back(tmpGrid);
//             }
//         }
//     }

//     return ret;
// }

};    // namespace graph_search
