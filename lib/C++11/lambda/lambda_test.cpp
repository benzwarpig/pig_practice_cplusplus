// for test lambda

// #include <iostream>
#include <spdlog/spdlog.h>

/*
    base struct

    [capture list] (parameter list) -> return type
    {

    }
*/

template <class Fn>
void CallLambdaFunc(Fn fn) {
    fn();
}

int main(void) {
    spdlog::info("start test lambda");

    // 普通lambda
    auto tmp = []() { spdlog::info("for simple test"); };
    CallLambdaFunc(tmp);

    // 值捕获
    // 值捕获编译器会在该匿名类中，生成一个私有变量，且在匿名对象构造时，将当前捕获的变量的值赋值给该私有对象
    {
        int i = 8;
        auto tmp_val =
            [i]()  // 默认是 const 的，对捕获的值赋值会报错，可以加 mutable
        {
            // spdlog::info("for value test,{}", i++); // error i is read-only
        };
        CallLambdaFunc(tmp_val);
        spdlog::info("for value test,{}", i);
    }

    // 可变值捕获
    {
        int i = 8;
        auto tmp_val =
            [i]() mutable  // 默认是 const 的，对捕获的值赋值会报错，可以加
                           // mutable
        { spdlog::info("for variable value test,{}", i++); };
        CallLambdaFunc(tmp_val);
        spdlog::info("for variable value test,{}", i);
    }

    // 引用捕获
    {
        int i = 8;
        auto tmp_val = [&i]() { spdlog::info("for ref test,{}", i++); };
        CallLambdaFunc(tmp_val);
        spdlog::info("for ref test,{}", i);
    }

    // 隐式值捕获
    {
        int i = 8;
        auto tmp_val = [=]() { spdlog::info("for Implicit Value test,{}", i); };
        CallLambdaFunc(tmp_val);
        spdlog::info("for Implicit Value test,{}", i);
    }

    // 隐式引用捕获
    {
        int i = 8;
        auto tmp_val = [&]() { spdlog::info("for Implicit Ref test,{}", i++); };
        CallLambdaFunc(tmp_val);
        spdlog::info("for Implicit Ref test,{}", i);
    }

    // 值 尾至返回
    {
        int i = 8;
        auto tmp_val = [=]() mutable -> int {
            spdlog::info("for Tail return Value test,{}", i++);
            return i;
        };
        spdlog::info("for Tail return Value test,{}", tmp_val());
    }

    // 引用 尾至返回
    {
        int i = 8;
        auto tmp_val = [&]() -> int {
            spdlog::info("for Tail return Ref test,{}", i++);
            return i;
        };
        spdlog::info("for Tail return Ref test,{}", tmp_val());
    }

    return 0;
}
