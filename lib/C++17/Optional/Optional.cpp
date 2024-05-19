#include "Optional.h"

// using MsgType = std::tuple<int, std::string>;

// pig_lib::PigOptional<MsgType> getTester(MsgType tmp) {
//     return pig_lib::make_optional<MsgType>(tmp);
// }

int main() {
    spdlog::info("PigOptional Tester !!!");

    // auto tmp = getTester(std::make_tuple<int, std::string>(1, "hello"));

    return 0;
}
