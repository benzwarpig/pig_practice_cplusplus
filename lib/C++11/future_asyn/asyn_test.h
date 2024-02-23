#pragma once

#include <spdlog/spdlog.h>

#include <future>
#include <iostream>

class MyThread {
private:
    std::thread m_Thread;
};

// file end