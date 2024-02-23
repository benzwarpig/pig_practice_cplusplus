#include "function_templete.h"

#include <spdlog/spdlog.h>  // third package

#include <functional>

void ShowMe(int a, double b) { spdlog::info("void ShowMe( {} , {} )", a, b); }

void CallShowMe(std::function<void(int a)> func) { func(1); }

template <typename Fn>
class PigFunction {
public:
    PigFunction() { spdlog::info("PigFunction() is Called"); }

    PigFunction(Fn&& func) : fn(func) {
        fn();
        spdlog::info("PigFunction( Fn&& func ) is Called");
    };
    ~PigFunction() { spdlog::info("~PigFunction is Called"); };

private:
    Fn fn;

};  // class PigFunction

class PigHelper {
public:
    template <typename T>
    auto operator<<(T&& t) {
        // spdlog::info( "func_type : {}", typeid( T ).name() );
        return PigFunction<T>(std::forward<T>(t));
    }
};

auto tmp_fn = []() -> int {
    spdlog::info("Call PigFunction");

    return 100;
};

int main() {
    // 1. 通过std::function 调用到lambda
    CallShowMe(std::bind(ShowMe, 1, 3.14));

    // 2. 通过模板
    PigHelper{} << std::bind(ShowMe, 2, 9.8);  // std::bind( ShowMe, 2, 9.8 )

    // 3. 自造std::function
    Functional aaa{tmp_fn};
    Functional bbb(std::move(aaa));

    spdlog::info("Show aaa");
    aaa();
    spdlog::info("Show bbb");
    bbb();

    return 0;
}
