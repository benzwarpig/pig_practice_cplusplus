#include "pig_signal.h"

class Tester {
public:
    Tester() { spdlog::info("Tester ctor"); };
    ~Tester() { spdlog::info("Tester dtor"); };
};

int main() {
    spdlog::info("pig_signal");
    Tester tester;
    // 接收到ctrl+c时的处理函数
    signal(SIGINT, [](int signum) {
        spdlog::info("receive SIGINT : {}", signum);
        exit(signum);
    });
    // 接收到浮点数异常时的处理函数(非法除0)
    signal(SIGFPE, [](int signum) {
        spdlog::info("receive SIGFPE : {}", signum);
        exit(signum);
    });

    pause();
}
