#include "pig_semaphore.h"

#include <csignal>
#include <memory>

#define kThreadNoName 1  // 线程间无名信号同步
#define kThreadName 2    // 线程间有名信号同步
#define kProcessName 3   // 进程间有名信号同步

#define SEM_TEST_OPTION kThreadNoName

int main() {
    spdlog::info("this is pig_semaphore_test");

    signal(SIGINT,
           [](int signum) { spdlog::info("receive SIGINT : {}", signum); });

#if defined(SEM_TEST_OPTION) && (SEM_TEST_OPTION == kThreadNoName)
    /***************************************************************************/
    /* 测试项：信号量 */
    /* 测试详情：无名信号量，线程间通信测试代码段 */
    bool threadRunning = true;
    Semaphore sem_tester;
    if (!sem_tester.open(0)) {
        spdlog::error("open failure !!!");
    }
    std::vector<std::future<int>> futures;
    auto futureA =
        std::async(std::launch::async, [&sem_tester, &threadRunning]() {
            while (threadRunning) {
                std::this_thread::sleep_for(
                    std::chrono::milliseconds(1000));  // 1s 发送一次
                if (!sem_tester.post()) {
                    spdlog::error("post failure !!!");
                }
                spdlog::info("sem_tester post once");
            }
            sem_tester.close();
            return -1;
        });
    futures.emplace_back(std::move(futureA));

    auto futureB =
        std::async(std::launch::async, [&sem_tester, &threadRunning] {
            while (threadRunning) {
                // spdlog::info("sem_tester waiting");
                if (!sem_tester.wait()) {
                    spdlog::error("wait failure !!!");
                }
                spdlog::info("sem_tester receive once");
            }
            sem_tester.close();
            return -1;
        });
    futures.emplace_back(std::move(futureB));

/***************************************************************************/
#elif defined(SEM_TEST_OPTION) && (SEM_TEST_OPTION == kThreadName)
    /***************************************************************************/
    /* 测试项：信号量 */
    /* 测试详情：有名信号量，线程间通信测试代码段 */
    bool threadRunning = true;
    constexpr auto semName = "CommonSem";
    std::vector<std::future<int>> futures;

    std::future<int> futureA = std::async(std::launch::async, [&threadRunning] {
        auto semA = std::make_unique<SemaphoreName>(semName);
        semA->open(1);
        while (threadRunning) {
            std::this_thread::sleep_for( 
                std::chrono::milliseconds(1000));  // 1s 发送一次
            // if (!semA->post()) {
            //     spdlog::error("post failure !!!");
            // }
            spdlog::info("sem_tester post once");
        }
        spdlog::error("debug {}", __LINE__);
        semA->close();
        return -1;
    });
    futures.emplace_back(std::move(futureA));

    std::future<int> futureB = std::async(std::launch::async, [&threadRunning] {
        auto semB = std::make_unique<SemaphoreName>(semName);
        semB->open(1);
        while (threadRunning) {
            spdlog::warn("debug : {}", (semB->get().__align));
            if (!semB->wait()) {
                spdlog::error("wait failure !!!");
            }
            spdlog::info("sem_tester receive once");
        }
        spdlog::error("debug {}", __LINE__);
        semB->close();
        return -1;
    });
    futures.emplace_back(std::move(futureB));

    /***************************************************************************/
#elif defined(SEM_TEST_OPTION) && (SEM_TEST_OPTION == kProcessName)

#endif

    pause();
    spdlog::warn("end pig_semaphore_test !!!");
    threadRunning = false;
    for (auto& tmp : futures) {
        if (tmp.valid()) {
            tmp.wait();
        }
    }
}
