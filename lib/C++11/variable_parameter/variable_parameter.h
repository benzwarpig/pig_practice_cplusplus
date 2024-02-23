#pragma once

#include <spdlog/spdlog.h>

#include <functional>
#include <iostream>
#include <string>
#include <type_traits>

/*****************************************************************
 * 可变参数模板
 *   - class variable parameter
 *      - 模版偏特化和递归方式来展开参数包
 *****************************************************************/
// 因为可变参数模板中的模板参数可以有0个
template <typename... Args>
struct Sum;

template <typename First, typename... Rest>
struct Sum<First, Rest...> {
    enum { value = Sum<First>::value + Sum<Rest...>::value };
};

// 递归终止
template <typename Last>
struct Sum<Last> {
    enum { value = sizeof(Last) };
};
/*****************************************************************/

/*****************************************************************
 * 可变参数模板
 *   - class variable parameter
 *      - 继承方式展开参数包
 *****************************************************************/
template <int...>
struct IndexSeq {};

template <int N, int... Indexes>
struct MakeIndexes : MakeIndexes<N - 1, N - 1, Indexes...> {};

template <int... Indexes>
struct MakeIndexes<0, Indexes...> {
    typedef IndexSeq<Indexes...> type;
};
/*****************************************************************/
template <class T, class... Args>
T* getInstance(Args&&... args) {
    return new T(std::forward<Args>(args)...);
}

class TopicMessage {
public:
    TopicMessage(std::string name, int time_out)
        : m_topicName(name), m_timeOut(time_out) {
        spdlog::info("Ctor TopicMessage name : {},time_out:{}", m_topicName,
                     m_timeOut);
    }

    ~TopicMessage() = default;

private:
    std::string m_topicName{""};
    int m_timeOut{0};
};

class ServiceMessage {
public:
    ServiceMessage(std::string name, int time_out, int times)
        : m_topicName(name), m_timeOut(time_out), m_times(times) {
        spdlog::info("Ctor ServiceMessage name : {},time_out:{},m_times:{}",
                     m_topicName, m_timeOut, m_times);
    }

    ~ServiceMessage() = default;

private:
    std::string m_topicName{""};
    int m_timeOut{0};
    int m_times{0};
};

// 可变参数模版实现泛化的delegate
//
template <class R, typename... Args>
class MyDelegate {
public:
    MyDelegate(R (*f)(Args...)) : m_f(f) {}

    R operator()(Args&&... args) { return (*m_f)(std::forward<Args>(args)...); }

private:
    R (*m_f)(Args...);
};

template <class R, typename... Args>
MyDelegate<R, Args...> CreateDelegate(R (*f)(Args...)) {
    return MyDelegate<R, Args...>(f);
}

struct A {
    void Fun(int i) { std::cout << i << std::endl; }
    void Fun1(int i, double j) { std::cout << i + j << std::endl; }
};
