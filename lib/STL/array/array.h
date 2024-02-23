#pragma once

// 没有ctor  没有dtor
template <typename _TP, std::size_t _Nm>
struct array {
    typedef _TP value_type;
    typedef _Tp* pointer;
    typedef value_type* iterator;

    // 支持 zero-sized arrays mandatory
    value_type _M_instance[_Nm ? _Nm : 1];

    iterator begin() { return iterator(&_M_instance[0]); }

    iterator end() { return iterator(&_M_instance[_Nm]); }
};
