#include "variable_parameter.h"

#include <initializer_list>

/*****************************************************************
 * 可变参数模板
 *   - 函数模板
 *     - 通过递归函数展开参数包(打印)
 *****************************************************************/
// 终止递归函数
template <class Args>
void print(Args args) {
    std::cout << "parameter " << args << std::endl;
}

// 接收一个和一包
template <class Head, class... Args>
void print(Head head, Args... rest) {
    std::cout << "parameter " << head << std::endl;
    print(rest...);  // 传进来一包，分解为一个和一包，然后递归调用
}
/*****************************************************************/

/*****************************************************************
 * 可变参数模板
 *   - 函数模板
 *     - 通过递归函数展开参数包(求和)
 *****************************************************************/
// 终止递归函数
template <class T>
T sum(T head) {
    return head;
}

// 接收一个和一包
template <class T, class... Args>
T sum(T head, Args... args) {
    return head + sum<T>(args...);
}

/*****************************************************************/

/*****************************************************************
 * 可变参数模板
 *   - 函数模板
 *     - 使用逗号表达式展开包
 *****************************************************************/
// 逗号表达式展开参数包是一种非递归方式，需要借助逗号表达式和初始化列表。
template <class T>
void printarg(T t) {
    std::cout << t << std::endl;
}

template <class... Args>
void expand(Args... args) {
    auto arr = {(printarg(args), 0)...};
}
/*****************************************************************/

/*****************************************************************
 * 可变参数模板
 *   - 函数模板
 *     - lambda func for variable_parameter
 *****************************************************************/
template <class F, class... Args>
void expandLambda(const F& f, Args&&... args) {
    std::initializer_list<int>{(f(std::forward<Args>(args)), 0)...};
}
/*****************************************************************/
void TestFunc(int i, int j) { spdlog::info("i:{},j:{}", i, j); }

int main() {
    // 测试递归打印
    print(1, 2.f, "3", '4');

    // 测试递归求和
    spdlog::info("sum : {}", sum(1, 2, 3, 4, 5));

    // 使用逗号表达式循环打印
    expand(1, 2, "3", 4, '5');

    // lambda func
    expandLambda([](auto i) { std::cout << i << std::endl; }, 6, 7, 8, '9',
                 "10");

    // variable class test
    auto packSize = Sum<int, int, double>::value;
    spdlog::info("packSize : {}", packSize);

    // inheritance expansion
    using T = MakeIndexes<5>::type;
    spdlog::info("type size : {}", typeid(T).name());

    // wxc test code
    // TopicMessage* topicObj =
    //     MessageFactory<TopicMessage>().Instance("topic", 500);

    auto* topicObj = getInstance<TopicMessage>("topic", 500);
    auto* serviceObj = getInstance<ServiceMessage>("service", 500, 3);

    // wxc test var fn
    A a;
    auto d = CreateDelegate(&TestFunc);
    d(1, 2);  //调用委托，将输出1

    while (1) {
    }
}
