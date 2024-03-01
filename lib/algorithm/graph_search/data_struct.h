#pragma once
#include <spdlog/spdlog.h>
#include <iostream>
#include <array>
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

    ~Grid2D() = default;
};

struct HashPoint {
    size_t operator()(const Grid2D &p) const { return (p.x + p.y * INT32_MAX); }
};

struct EqPoint {
    bool operator()(const Grid2D &p1, const Grid2D &p2) const {
        return (p1.x == p2.x && p1.y == p2.y);
    }
};

static std::vector<Grid2D> get4Neighbors(int step = 1) {
    return std::vector<Grid2D>{Grid2D(step, 0), Grid2D(0, step), Grid2D(-step, 0),
                               Grid2D(0, -step)};
}

static std::vector<Grid2D> get8Neighbors(int step = 1) {
    return std::vector<Grid2D>{
        Grid2D(step, 0),  Grid2D(step, step),   Grid2D(0, step),  Grid2D(-step, step),
        Grid2D(-step, 0), Grid2D(-step, -step), Grid2D(0, -step), Grid2D(step, -step)};
}

struct Map {
    const int kCol = 10;
    const int kRow = 20;
    uint8_t map_[10][20];

    Grid2D getCenterGrid() { return {kCol / 2, kRow / 2}; }

    void loadMap(uint8_t tmp[10][20]) {
        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 20; j++) {
                map_[i][j] = tmp[i][j];
            }
        }
    }
};

};    // namespace graph_search
