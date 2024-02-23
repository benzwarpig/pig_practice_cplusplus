#pragma once

#include <spdlog/spdlog.h>

template <class I>
struct iterator_traits
{
    typedef typename I::iterator_category iterator_category;
    typedef typename I::value_type value_type;
    typedef typename I::difference_type difference_type;
    typedef typename I::pointer pointer;
    typedef typename I::reference reference;
};

template <class T>
struct iterator_traits<T *>
{
    typedef random_access_iterator_tag iterator_category;
    typedef T value_type;
    typedef ptrdiff_t difference_type;
    typedef T *pointer;
    typedef T &reference;
};

template <class T>
struct iterator_traits<const T *>
{
    typedef random_access_iterator_tag iterator_category;
    typedef T value_type;
    typedef ptrdiff_t difference_type;
    typedef const T *pointer;
    typedef const T &reference;
};

#if 0
// 2
template <template _Iter>
inline typename iterator_traits<_Iter>::iterator_category
__iterator_category(const _Iter &)
{
    return typename iterator_traits<_Iter>::iterator_category();
}

// 3
template<typename _RandomAccessIterator>
void __rotate(_RandomAccessIterator __first,
              _RandomAccessIterator __middle,
              _RandomAccessIterator __last,
              random_access_iterator_tag)
{
...
    typedef typename iterator_traits<_RandomAccessIterator>::difference_type _Distance;
    typedef typename iterator_traits<_RandomAccessIterator>::value_type _ValueType;
    _Distance __n = __last - __first;
    _Distance __k = __middle - __first;
...

    for(;;)
    {
        if(__k < __n - __k)
        {
            if(__is_pod(_ValueType) && __k == 1)
            {
                __ValueType __t = _GLIBCXX_MOVE(*__P);
            }
        }
    }
...
}

#endif

// 1
template <typename _ForwardIterator>
inline void
rotate(_ForwardIterator __first,
       _ForwardIterator __middle,
       _ForwardIterator __last)
{
    std::__rotate(__first, __middle, __last,
                  std::__iterator_category(__first));
}

// stl_traits.h