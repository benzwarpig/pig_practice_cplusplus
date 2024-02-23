#pragma once

#include <spdlog/spdlog.h>

#include <cmath>     // NOLINT
#include <iostream>  // NOLINT
#include <string>    // NOLINT
#include <utility>   // NOLINT
#include <vector>    // NOLINT

class Point2D {
public:
    Point2D() = default;
    Point2D(float x_, float y_) : m_x(x_), m_y(y_) {}

    void display(void) {
        std::cout << "m_x : " << m_x << "   "
                  << "m_y : " << m_y << std::endl;
    }

    float x() const { return m_x; }
    float y() const { return m_y; }

    void set_x(float x) { m_x = x; }
    void set_y(float y) { m_y = y; }

public:
    float m_x;
    float m_y;
};

class AreaInterpolation {
public:
    explicit AreaInterpolation(const std::vector<Point2D>& points,
                               double step = k_step)
        : m_step(step), m_points(points) {}

    explicit AreaInterpolation(std::vector<Point2D>&& points,
                               double step = k_step)
        : m_step(step), m_points(std::move(points)) {}

    ~AreaInterpolation() {}

public:
    std::vector<Point2D> getAreaPoints() {
        auto maxSize = m_points.size();
        if (maxSize <= 1) {
            std::cerr << "too less points" << std::endl;
            return {};
        }
        // 处理边界场景
        auto DealNum = [&](int i) { return (i == maxSize) ? 0 : i; };
        std::vector<Point2D> points;
        for (decltype(maxSize) i = 0; i < maxSize; i++) {
            auto first = m_points.at(DealNum(i));
            auto second = m_points.at(DealNum(i + 1));
            double slope = getTwoPointSlope(first, second);
            int number = getTwoPointDistance(first, second) / m_step;
            float tmpX = first.x();
            float tmpY = first.y();
            Point2D tmPoint;
            for (int j = 0; j < number; j++) {
                tmpX += m_step * cosf(slope);
                tmpY += m_step * sinf(slope);
                tmPoint.set_x(tmpX);
                tmPoint.set_y(tmpY);
                points.emplace_back(tmPoint);
            }
        }
        return points;
    }

private:
    // 获得两点间斜率
    double getTwoPointSlope(Point2D& first, Point2D& second) {
        return atan2((second.y() - first.y()), (second.x() - first.x()));
    }
    // 获得两点间直线距离
    double getTwoPointDistance(Point2D& first, Point2D& second) {
        return sqrt(pow((first.x() - second.x()), 2) +
                    pow((first.y() - second.y()), 2));
    }

private:
    double m_step;

    std::vector<Point2D> m_points;

private:
    static constexpr const auto k_pi = 3.1415926;
    static constexpr const auto k_step = 0.05;
};
