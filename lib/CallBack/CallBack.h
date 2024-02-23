#ifndef __CALLBACK_H
#define __CALLBACK_H

#include <spdlog/spdlog.h>

#include <iostream>
#include <vector>

/*!
    @brief : slot 即槽函数，内部管理着一个函数指针，
    外部通过Execute触发该函数指针，实现回调功能
*/
template <class T, class T1>
class slot {
public:
    slot(T *pObj, void (T::*pMemberFunc)(T1)) {
        m_pObj = pObj;
        m_pMemberFunc = pMemberFunc;
    }

    void Execute(T1 para) { (m_pObj->*m_pMemberFunc)(para); }

private:
    T *m_pObj;
    void (T::*m_pMemberFunc)(T1);

public:
    ~slot(){};

private:
    slot(const slot &) = delete;
    slot &operator+(const slot &) = delete;
};

/*!
    @brief : msignal通过vector管理信号槽slot，提供connect与operator()
    两个接口，connect将监听者挂在到一个slot上，operator()会依次调用slot上
    订阅者的回调函数。
*/
template <class T, class T1>
class msignal {
public:
    void connect(T *pObj, void (T::*pMemberFunc)(T1)) {
        m_slots.push_back(new slot<T, T1>(pObj, pMemberFunc));
    }

    void operator()(T1 para) {
        for (auto &tmp : m_slots) {
            tmp->Execute(para);
        }
    }

    ~msignal() {
        for (auto &tmp : m_slots) {
            delete tmp;
        }
    }

private:
    std::vector<slot<T, T1> *> m_slots;

public:
    msignal(){};

private:
    msignal(const msignal &) = delete;
    msignal &operator+(const msignal &) = delete;
};

class Subscriber {
public:
    void CallBackA(int a) { spdlog::info("CallBackA :{}", a); }

    void CallBackB(int b) { spdlog::info("CallBackB :{}", b); }

public:
    Subscriber(){};
    ~Subscriber(){};

private:
    Subscriber(const Subscriber &) = delete;
    Subscriber &operator+(const Subscriber &) = delete;
};

/*!
    @brief : 发布者Publisher管理信号msignal，对外暴露set接口
    外部调用set时出发成员对象msignal动作。
*/
template <class T, class T1>
class Publisher {
public:
    inline int GetValue() { return m_value; };
    inline void SetValue(int new_value) {
        if (new_value != m_value) {
            m_value = new_value;
            m_sig(m_value);
        }
    }

    msignal<T, T1> m_sig;

private:
    int m_value;

public:
    Publisher() : m_value(0){};
    ~Publisher(){};

private:
    Publisher(const Publisher &) = delete;
    Publisher &operator+(const Publisher &) = delete;
};

#endif  //__CALLBACK_H