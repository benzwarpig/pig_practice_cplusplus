#include <spdlog/spdlog.h>

#include <array>
#include <random>
#include <string>
#include <vector>

struct CityAttribute {
    /* data */
    int cost;
    int happy;
};

constexpr const auto n = 10u;
constexpr const auto k = 100u;

int main(void) {
    std::default_random_engine e;
    std::uniform_int_distribution<int> u(1, 100);  // 左闭右闭区间
    e.seed(time(0));

    std::vector<CityAttribute> city_map;

    int city_index = 0;
    for (int i = 0; i < n; i++) {
        CityAttribute temp;
        temp.cost = u(e);
        temp.happy = u(e);
        city_map.push_back(temp);
    }

    std::sort(city_map.begin(), city_map.end(),
              [](const CityAttribute& tmp1, const CityAttribute& tmp2) -> bool {
                  return tmp1.happy > tmp2.happy;
              });

    city_index = 0;
    for (auto& tmp : city_map) {
        spdlog::info("{} :  {} {} ", city_index++, tmp.cost, tmp.happy);
    }

    int already_cost = 0;
    int max_happy = 0;
    uint8_t number = 0;

    for (int i = 0; i < n; i++) {
        if ((city_map.at(i).cost + already_cost) > k) {
            continue;
        }
        already_cost += city_map.at(i).cost;
        max_happy += city_map.at(i).happy;

        if (++number == 2) {
            break;
        }
    }

    spdlog::info("under limit cost {}, max happy is {}", already_cost,
                 max_happy);

    return 0;
}
