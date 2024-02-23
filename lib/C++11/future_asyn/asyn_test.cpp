/**
 * @file asyn_test.cpp
 * @author benzs_war_pig (benzwarpig@outlook.com)
 * @brief 测试stl异步编程库
 * @version 1.0
 * @date 2022-08-29
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "asyn_test.h"

void PromiseTester(std::promise<int>& tmp, int val) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    spdlog::info("Enter PromiseTester , the val : {}", val);
    val *= 10;
    tmp.set_value_at_thread_exit(val);
}

void PromiseTesterDelay(std::future<int>& tmp, int val) {
    tmp.wait();
    spdlog::info("PromiseTesterDelay , the val : {}", val);
}

//计算阶乘
int factorial(int n) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    if (n == 1) {
        return 1;
    }

    return n * factorial(n - 1);
}

// 测试函数
int AsyncTester(int n) {
    spdlog::info("Enter AsyncTester");
    std::this_thread::sleep_for(std::chrono::milliseconds(n * 1000));
    spdlog::info("Quit AsyncTester");
    return n;
}

int main() {
    spdlog::info("asyn_test");
    std::cout << std::endl;
#if false
    /** 测试std::future 和 std::promise ****************************************************************************************************************************/
    spdlog::info( "Start std::promise test **********************************************************************" );
    // 主线程等待子线程执行完毕
    std::promise< int > tmp_promise_1;
    std::future< int >  tmp_future_1 = tmp_promise_1.get_future();

    std::thread promise_thread_1( PromiseTester, std::ref( tmp_promise_1 ), 1 );
    promise_thread_1.join();

    spdlog::info( "promise_1 tester , the val is : {}", tmp_future_1.get() );

    // 子线程等待主线程执行完毕
    std::promise< int > tmp_promise_2;
    std::future< int >  tmp_future_2 = tmp_promise_2.get_future();
    std::thread         promise_thread_2( PromiseTesterDelay, std::ref( tmp_future_2 ), 2 );
    spdlog::info( "PromiseTesterDelay Thread is Running" );
    std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
    tmp_promise_2.set_value( 100 );
    promise_thread_2.join();
    std::cout << std::endl;

    /** 测试std::future 和 std::package_task ****************************************************************************************************************************/
    spdlog::info( "Start std::package_task test **********************************************************************" );
    // 直接将 std::package_task 当做函数对象使用
    std::packaged_task< int( int, int ) > package_1( []( int i, int j ) -> int {
        spdlog::info( "i + j : {}", i + j );
        return i + j;
    } );
    std::future< int >                    future_package_1 = package_1.get_future();
    package_1( 1, 2 );
    spdlog::info( "package_1 num : {}", future_package_1.get() );

    // 将std::packaged_task作为任务传递给std::thread线程
    std::packaged_task< int( int ) > task2( factorial );
    std::future< int >               future_package_2 = task2.get_future();
    std::thread                      t3( std::ref( task2 ), 7 );
    t3.join();
    // spdlog::info( "pack2 : {}", future_package_2.get() );
#endif
    /** 测试std::futrue 和 std::async
     * *****************************************************************************************************/
    // 测试 std::launch::async 异步策略
    std::future<int> future_package_3 =
        std::async(std::launch::async, AsyncTester, 5);
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    future_package_3.wait();
    spdlog::info("set future_package_3 val : {}", future_package_3.get());

    // 测试 std::launch::deferred 异步策略
    std::future<int> future_package_4 =
        std::async(std::launch::deferred, AsyncTester, 5);
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    future_package_4.wait();
    spdlog::info("set future_package_3 val : {}", future_package_4.get());

    while (1) {
        /* code */
    }
}