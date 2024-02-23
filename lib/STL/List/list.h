#pragma once

#include <memory>

template <class T>
struct __list_node {
    typedef void *void_pointer;
    void_pointer prev;
    void_pointer next;
    T data;
};

template <class T, class Ref, class Ptr>
struct __list_iterator {
    typedef __list_iterator<T, Ref, Ptr> self;
    typedef bidirectional_iterator_tag iterator_category;  // 表示方向

    typedef T value_type;   //
    typedef Ptr pointer;    //
    typedef Ref reference;  //

    typedef __list_node<T> *link_type;
    typedef ptrdiff_t difference_type;  //

    link_type node;

    reference operator*() const { return (*node).data; }

    pointer operator->() const { return &(operator*()); }

    self &operator++() {
        node = (link_type)(((*node).next));
        return *this;
    }

    self operator++(int) {
        self tmp =
            *this;  // self tmp 意味着 __list_iterator(const iterator& x) :
                    // node(x.node) {}，即已经认为是ctor
                    // *this不会触发operator*() 操作，因为它被解释称了const
                    // iterator& x的实参
                    // 原因是编译器会先遇见=，=本身会隐式调用cptr，导致*this会被解释为cptr的实参。
        ++*this;  // self &operator++() {node = (link_type)((*node).next);}
        return tmp;
    }
};

// sizeof(list<T>) = 4;
template <class T, class Alloc = std::allocator<T>>
class list {
protected:
    typedef __list_node<T> list_node;

public:
    typedef list_node *link_type;
    typedef __list_iterator<T, T &, T *> iterator;

protected:
    link_type node;
};

// list.h