#include "ObserverMode.h"

enum ObserverTest {
    kFirst = (0),
    kSecond,
    kThird,
};

int main(void) {
    Subject subject;

    auto m_subject1 = subject.createModuleObserver<int>(
        [](const int& tmp) { spdlog::info("value : {}", std::to_string(tmp)); },
        [](const int& tmp) {}, [](const int& tmp) {}, [](const int& tmp) {});

    m_subject1->setModuleState(6);

    auto m_subject2 = subject.createModuleObserver<ObserverTest>(
        [](const ObserverTest& tmp) {
            spdlog::info("value : {}", std::to_string(tmp));
        },
        [](const ObserverTest& tmp) {}, [](const ObserverTest& tmp) {},
        [](const ObserverTest& tmp) {});

    subject.onStart();

    pause();
    return 0;
}
