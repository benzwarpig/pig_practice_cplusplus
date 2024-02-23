#pragma once

#include <spdlog/spdlog.h>

#include <iostream>
#include <string>
#include <tuple>

namespace pig_lib {

#if __cplusplus < 201703L
template <typename T>
class PigOptional {
public:
    PigOptional() = default;

    PigOptional(const PigOptional&) = default;
    PigOptional& operator=(const PigOptional&) = default;

    PigOptional(PigOptional&&) = default;
    PigOptional& operator=(PigOptional&&) = default;

    ~PigOptional() = default;

    template <typename... Args>
    explicit PigOptional(Args&&... args)
        : m_value(true, T(std::forward<Args>(args)...)) {
        spdlog::info("call PigOptional(Args&&... args) , args_size:{}",
                     sizeof...(args));
    }

    template <typename... Args>
    explicit PigOptional(bool cond, Args&&... args)
        : m_value(cond, T(std::forward<Args>(args)...)) {
        spdlog::info(
            "call PigOptional(bool cond, Args&&... args) , args_size:{}",
            sizeof...(args));
    }

    explicit operator bool() const { return m_value.first; }

    T& value() { return m_value.second; }

    const T& value() const { return m_value.second; }

    T* operator->() { return &(m_value.second); }

    const T* operator->() const { return &(m_value.second); }

    T& operator*() { return m_value.second; }

    const T& operator*() const { return m_value.second; }

private:
    std::pair<bool, T> m_value;
};

template <class T>
inline PigOptional<T> make_optional(T const& v) {
    return PigOptional<T>(v);
}

template <class T>
inline PigOptional<T> make_optional(bool cond, T const& v) {
    return PigOptional<T>(cond, v);
}

struct nullopt_t {
    explicit constexpr nullopt_t(int) {}
};
#endif

};  // namespace pig_lib

// PigOptional.h
