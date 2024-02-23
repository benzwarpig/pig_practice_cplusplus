#pragma once

#include <semaphore.h>
#include <spdlog/spdlog.h>

#include <future>
#include <string>

#include "pig_semaphore_impl.h"

class Semaphore : public SemaphoreBase {
public:
    Semaphore() = default;
    ~Semaphore() = default;

    bool open(int value = -1) override {
        bool ret = true;
        if (value < 0) {
            ret = false;
        }
        /* 初始化无名信号量 */
        if (-1 == sem_init(&m_sem, 0, value)) {
            ret = false;
            spdlog::error("sem init error:{}", value);
        }
        return ret;
    }

    bool close() override {
        bool ret = true;
        if (sem_destroy(&m_sem) != 0) {
            ret = false;
            spdlog::error("sem close error");
        }
        return ret;
    }
};

class SemaphoreName : public SemaphoreBase {
public:
    SemaphoreName() = delete;
    SemaphoreName(const char* name) {
        if (name != NULL) {
            m_name = std::string(name);
        } else {
            spdlog::error("name sem ctor falut");
        }
    };
    ~SemaphoreName() = default;

    bool open(int value = -1) override {
        bool ret = true;
        m_name = std::string("/") + m_name;
        sem_t* sem = SEM_FAILED;
        sem = sem_open(m_name.c_str(), O_CREAT | O_EXCL | O_RDWR, value);
        spdlog::info("name : {}", m_name.c_str());
        if (sem == SEM_FAILED) /* 信号量已存在 */
        {
            if (value < 0) {
                sem = sem_open(m_name.c_str(), 0);
            } else {
                unlink();
                sem = sem_open(m_name.c_str(), O_CREAT | O_EXCL | O_RDWR, 0664,
                               value);
            }
        }
        if (sem == SEM_FAILED) {
            spdlog::error(" LINE: {}", __LINE__);
        }
        m_sem.__align = sem->__align;
        spdlog::info("m_sem.__align:{}", m_sem.__align);
        return ret;
    }

    bool close() override {
        bool ret = true;
        if (sem_close(&m_sem) != 0) {
            spdlog::error("sem close error : {}", strerror(errno));
            ret = false;
        }
        return ret;
    }

    bool unlink() {
        bool ret = true;
        if (sem_unlink(m_name.c_str()) != 0) {
            spdlog::error("sem unlink({}) , error:{}", m_name.c_str(),
                          strerror(errno));
            ret = false;
        }
        return ret;
    }

private:
    std::string m_name{""};
};
