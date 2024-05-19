#include "any.h"
#include "TestParam.h"

#include "any_helper.hpp"

std::any a = 1;

int main() {
    spdlog::info("any Tester !!!");

    // 打印a的类型
    spdlog::info("a type is {:s}", std::string{a.type().name()});

    a = 3.14;
    spdlog::info("a type is {:s}", std::string{a.type().name()});

    a = true;
    spdlog::info("a type is {:s}", std::string{a.type().name()});

    // 异常的类型转换
    try {
        a = 1;
        std::cout << std::any_cast<float>(a) << "\n";
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
    }

    /**< wxc test std::any ************************************/
    // load param
    TestModeAny test1;
    {
        test1.mode = TestMode::kMode1;
        TestParamMode1 test1_param;
        test1_param.common.name = "test1";
        test1_param.common.index = 1;
        test1_param.test1 = 101;
        test1.param = test1_param;
    }

    // unpack param
    {
        std::any param = test1.param;
        TestParamMode1 tmp_param = std::any_cast<TestParamMode1>(param);
        spdlog::info("test1 common name:{},index:{}", tmp_param.common.name,
                     tmp_param.common.index);
    }

    /**< wxc test boost::any ************************************/
    TestModeBoostAny test1_boost;
    {
        test1_boost.mode = TestMode::kMode1;
        TestParamMode1 test1_param;
        test1_param.common.name = "boost_test1";
        test1_param.common.index = 1;
        test1_param.test1 = 101;
        test1_boost.param = any_helper::Type2Any(test1_param);
    }

    // unpack param
    {
        boost::any param = test1_boost.param;
        TestParamMode1 tmp_param;
        if (any_helper::Any2Type<TestParamMode1>(param, tmp_param)) {
            spdlog::info("test1_boost common name:{},index:{},type_name:{}",
                         tmp_param.common.name, tmp_param.common.index,
                         any_helper::GetAnyTypeName(param));
        } else {
            spdlog::error("test1_boost unsafe cast !!!");
        }
    }

    return 0;
}