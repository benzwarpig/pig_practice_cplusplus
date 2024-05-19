#pragma once

#include <spdlog/spdlog.h>

#include <iostream>
#include <string>
#include <tuple>

#include <any>
#include <boost/any.hpp>

enum class TestMode {
    kMode1 = (1),    // mode 1
    kMode2,          // mode 2
    kMode3,          // mode 3
    kMode4,          // mode 4
    kMode5           // mode 5
};

struct TestModeAny {
    TestMode mode;
    std::any param;
};

struct TestModeBoostAny {
    TestMode mode;
    boost::any param;
};
