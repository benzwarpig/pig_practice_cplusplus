#pragma once

#include <semaphore.h>

const constexpr char* kCommonName = "CommonSemaphore";

class SemaphoreBase {
public:
    SemaphoreBase() = default;
    virtual ~SemaphoreBase() = default;

    // 开启一个信号量
    virtual bool open(int value = -1) = 0;

    // 关闭一个信号量
    virtual bool close() = 0;

    // 等待信号
    bool wait() {
        bool ret = false;
        if (0 == sem_wait(&m_sem)) {
            ret = true;
        }
        return ret;
    }

    // 尝试等待
    bool tryWait() {
        bool ret = false;
        if (0 == sem_trywait(&m_sem)) {
            ret = true;
        }
        return ret;
    }

    // 发送信号
    bool post() {
        bool ret = false;
        if (0 == sem_post(&m_sem)) {
            ret = true;
        }
        return ret;
    }

    // 获取信号值
    bool getValue(int* value) {
        bool ret = true;
        if (-1 == sem_getvalue(&m_sem, value)) {
            ret = false;
        }
        return ret;
    }

    const sem_t& get() { return m_sem; }

protected:
    sem_t m_sem;
};
