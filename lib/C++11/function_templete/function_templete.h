#pragma once

#include <utility>  // std::move
class FunctionBase {
public:
    virtual ~FunctionBase() = default;
    virtual void CallMe() = 0;

};  // class FunctionBase

template <typename T>
class FunctionImpl : public FunctionBase {
public:
    FunctionImpl(T fn) : fn_(fn){};

    void CallMe() override {
        if (fn_) fn_();
    };

private:
    T fn_;
};  // class FunctionImpl

class Functional {
public:
    template <typename T>
    Functional(T&& fn) {
        function_impl = new FunctionImpl<T>(fn);
    }

    Functional(Functional& other) = delete;
    Functional(Functional&& other) {
        if (other.function_impl != function_impl) {
            swap(std::move(other));
            other.function_impl = nullptr;
        }
    }

    ~Functional() {
        if (function_impl) {
            delete function_impl;
        }
    }

public:
    void operator()() {
        if (function_impl) function_impl->CallMe();
    }

    void swap(Functional&& other) {
        auto* func_ptr = this->function_impl;
        this->function_impl = other.function_impl;
        other.function_impl = func_ptr;
    }

private:
    FunctionBase* function_impl = nullptr;
};  // class Functional

// function_templete.h