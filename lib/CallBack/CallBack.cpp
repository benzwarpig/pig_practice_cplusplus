#include "CallBack.h"

#include <unistd.h>

#include <iostream>

int main() {
    spdlog::info("CallBack Test");

    Subscriber r1;
    Subscriber r2;

    // 局限性：Publisher在实例化的时候，必须要知道Subscriber的类型，且
    // 只能通知单类型的通知，改进的办法是讲订阅者类型推导延迟到调用connect时
    Publisher<Subscriber, int> s;

    s.m_sig.connect(&r1, &Subscriber::CallBackA);
    s.m_sig.connect(&r1, &Subscriber::CallBackB);
    s.m_sig.connect(&r2, &Subscriber::CallBackA);
    s.m_sig.connect(&r2, &Subscriber::CallBackB);

    s.SetValue(2);

    pause();
}