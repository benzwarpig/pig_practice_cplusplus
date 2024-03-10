#include "area_interpolation.h"

#include <map>
#include <sstream>
#include <string>
#include <unordered_map>

namespace monitor {

enum class TestType {
    kTest1 = 1,
    kTest2,
    kTest3,
    kTest4,
    kTest5,
    kTest6,
    kTest7,
    kTest8,
    kTest9
};

static std::string Type2String(TestType type) {
    switch (type) {
        case TestType::kTest1:
            return "kTest1";
            break;

        case TestType::kTest2:
            return "kTest2";
            break;

        case TestType::kTest3:
            return "kTest3";
            break;

        case TestType::kTest4:
            return "kTest4";
            break;

        case TestType::kTest5:
            return "kTest5";
            break;

        case TestType::kTest6:
            return "kTest6";
            break;

        case TestType::kTest7:
            return "kTest7";
            break;

        case TestType::kTest8:
            return "kTest8";
            break;

        case TestType::kTest9:
            return "kTest9";
            break;

        default:
            break;
    }
}

struct TestStruct {
    TestType type;
    int id;
};

struct Pair {
    std::string key;
    std::string value;
};

struct Pairs {
    std::string key;
    std::vector<Pair> value;
};

};    // namespace monitor

namespace {
static std::vector<std::string> split(const std::string& value, const std::string& tmp) {
    std::vector<std::string> result;
    std::istringstream iss(value);
    std::string token;

    while (std::getline(iss, token, tmp[0])) {
        result.push_back(token);
    }

    return result;
}
};    // namespace

static std::map<monitor::TestType, std::vector<monitor::TestStruct>>
    collision_detail_counter_;

monitor::Pairs loadToPairs() {
    monitor::Pairs wxc_test;
    wxc_test.key = "wxc_test";
    auto& value_vec = wxc_test.value;

    for (const auto& tmp : collision_detail_counter_) {
        monitor::Pair pair;
        pair.key = std::move(Type2String(tmp.first));
        pair.value = "";
        for (const auto& value : tmp.second) {
            pair.value += "[" + Type2String(value.type) + "]" + ",";
            pair.value += "[" + std::to_string(value.id) + "]";
            pair.value += ";";
        }
        const auto& keys = split(pair.value, ";");
        std::cout << "current type is " << pair.key << std::endl;
        for (const auto& ttmp : keys) {
            std::cout << ttmp << std::endl;
        }
    }
    return wxc_test;
}

void UpdateCollisionDetail(const monitor::TestStruct& info) {
    using CollisionState = decltype(info.type);

    // 过滤掉无需统计的状态
    // if (info.type == CollisionState::kTest1) {
    //     return;
    // }

    auto InfoInsert = [info](const monitor::TestStruct& tmp) {
        auto search = collision_detail_counter_.find(tmp.type);
        if (search == collision_detail_counter_.end()) {
            // 如果从未插入过
            std::vector<monitor::TestStruct> collision_list;
            collision_list.push_back(info);
            collision_detail_counter_.insert({info.type, std::move(collision_list)});
            auto search_1 = collision_detail_counter_.find(info.type);
            spdlog::info("如果从未插入过,{}", search_1->second.size());
        } else {
            // 如果之前插入过
            search->second.push_back(info);
            spdlog::info("如果之前插入过,{}", search->second.size());
            for (const auto& tmp : search->second) {
                spdlog::info("tmp,id:{}", tmp.id);
            }
        }
    };
    InfoInsert(info);
}

int main() {
    // spdlog::info("enter area_inter once");

    // std::vector<Point2D> points;
    // Point2D tmPoint;
    // tmPoint.set_x(0);
    // tmPoint.set_y(0);
    // points.emplace_back(tmPoint);

    // tmPoint.set_x(0);
    // tmPoint.set_y(10);
    // points.emplace_back(tmPoint);

    // tmPoint.set_x(10);
    // tmPoint.set_y(10);
    // points.emplace_back(tmPoint);

    // tmPoint.set_x(0);
    // tmPoint.set_y(10);
    // points.emplace_back(tmPoint);

    // spdlog::info("points : {}", points.size());
    // AreaInterpolation areaBuilder(points);
    // auto areaPoints = areaBuilder.getAreaPoints();
    // for (const auto& point : areaPoints) {
    // }

    monitor::TestStruct test1{monitor::TestType::kTest1, 1};
    monitor::TestStruct test2{monitor::TestType::kTest2, 2};
    monitor::TestStruct test3{monitor::TestType::kTest3, 3};
    monitor::TestStruct test4{monitor::TestType::kTest4, 4};
    monitor::TestStruct test5{monitor::TestType::kTest5, 5};
    monitor::TestStruct test6{monitor::TestType::kTest5, 6};
    monitor::TestStruct test7{monitor::TestType::kTest5, 7};
    monitor::TestStruct test8{monitor::TestType::kTest5, 8};

    UpdateCollisionDetail(test1);
    UpdateCollisionDetail(test2);
    UpdateCollisionDetail(test3);
    UpdateCollisionDetail(test4);
    UpdateCollisionDetail(test5);
    UpdateCollisionDetail(test6);
    UpdateCollisionDetail(test7);
    UpdateCollisionDetail(test8);

    loadToPairs();

    return 0;
}