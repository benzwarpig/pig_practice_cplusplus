#pragma once

#include <memory>

template <class T, class Alloc = std::allocator<T>>
class vector {
public:
    typedef T value_type;
    typedef value_type *iterator;
    typedef value_type &reference;
    typedef size_t size_type;

protected:
    iterator start;
    iterator finish;
    iterator end_of_storage;

public:
    iterator begin() { return start; }

    iterator end() { return finish; }

    size_type size() const {
        return size_type(
            end() -
            begin());  // 只有当两个指针都指向同一个数组中的元素时，才允许从一个指针减去另一个指针。
    }

    size_type capacity() const { return size_type(end_of_storage() - begin()); }

    bool empty() const { return begin() == end(); }

    reference operator[](size_type n) { return *(begin() + n); }

    reference front() { return *begin(); }

    reference back() { return *end(); }

public:
    template <class T, class Alloc>
    void insert_aux(iterator position, const T &x) {
        if (finish != end_of_storage)  // 尚有未使用空间
        {
            // 在备用空间起始处建立一个元素，并以vector最后一个元素值作为其初值
            construct(finish, *(finish - 1));
            ++finish;

            T x_copy = x;

            copy_backward(position, finish - 2, finish - 1);
            *position = x_copy;
        } else  // 已无备用空间
        {
            constexpr size_type old_size = size();
            constexpr size_type len = (old_size != 0) ? 2 * old_size : 1;

            iterator new_start = data_allocator::allocate(len);
            iterator new_finish = new_start;

            try {
                // 将原vector的内容拷贝到新的vector
                new_finish = uninitialized_copy(start, position, new_start);
                construct(new_finish, x);  // 将新元素插入数组
                ++new_finish;

                // 如果是vector，那么就不需要在push_back之后再去把插入点（position）后面的元素依次拷贝
                // 但是如果是map，调用insert(p,x)，插入到p =
                // size()/2的位置上。这时就不仅需要把p之前的
                // 拷贝到新地址处，p之后的也需要拷贝过去，只是新值了p位置处的元素
                new_finish = uninitialized_copy(position, finish, new_finish);
            } catch (...) {
                destroy(new_start, new_finish);
                data_allocator::deallocate(new_start, len);
                throw;
            }
            // 释放原vector
            destroy(begin(), end());
            deallocate();

            start = new_start;
            finish = new_finish();
            end_of_storage = new_start + new_finish;
        }
    }

    void push_back(const T &x) {
        if (finish != end_of_storage) {
            construct(finish, x);
            ++finish;
        } else  // 已无可用空间
        {
            insert_aux(end(), x);
        }
    }
};
