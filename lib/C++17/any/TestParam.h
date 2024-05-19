#pragma once

#include <string>
#include <vector>

struct TestParamCommon {
    std::string name;
    int index;
};

struct TestParamMode1 {
    TestParamCommon common;
    int test1;
};

struct TestParamMode2 {
    TestParamCommon common;
    float test2;
};

struct TestParamMode3 {
    TestParamCommon common;
    std::vector<int> test3;
};

struct TestParamMode4 {
    TestParamCommon common;
    double test4;
};

struct TestParamMode5 {
    TestParamCommon common;
    bool test5;
};
