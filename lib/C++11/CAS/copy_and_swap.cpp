#include <pthread.h>
#include <spdlog/spdlog.h>
#include <unistd.h>

#include <atomic>
#include <deque>
#include <iostream>
#include <thread>
#include <vector>

/**
 * @brief 什么是CAS（copy and swap）？
 *
 * @note
 * CAS是一种在多线程开发中保证变量原子性的一种方式，它的底层实现是，有3个值
 *            内存值V    旧的预期值A     要修改的值B
 *        执行逻辑为：如果内存值V不等于旧的预期值A，那么说明内存值V已经被别的线程修改过了，此时不需要修改
 *                如果内存值V等于旧的预期值A，那么说明内存值V没有被修改，此时把要修改的新值B付给内存值V
 *        返回值：替换成功返回true，否则返回false
 *
 * @details
 * CAS的底层原理是乐观锁，也可以说是自旋锁，并不是无锁的，它的底层是通过__asm__volatile(LOCK_IF_MP(%4)“cmpxchgl”
 * %1,(%3))
 *          来实现CAS特性，但是"cmpxchgl"并不是原子特性的，这点可以从LOCK_IS_MP的定义看出端倪
 *          #defien LOCK_IF_MP(mp)"cmp $0," #mp ";je 1f;lock;1:"
 *          这句话的意思是说，如果是单核CPU那么就不需要锁定，如果是多核CPU就需要在前面加lock，所以汇编指令其实
 *          是lock
 * cmpxchgl指令，意识是如果是多核CPU，在执行cmpxchgl指令时，会去锁定北桥信号（为什么不是总线
 *          而是北桥信号，是因为锁定北线信号比总线要轻量一些）lock
 * cmpxchgl指令执行期间该缓存行会一直被锁定，其他
 *          处理器无法读写该指令要访问的内存区域，从而保证原子性
 */

/**
 * @brief 锁机制之悲观锁与乐观锁
 *
 * @note  悲观锁：
 *          -假定会发生并发冲突，屏蔽一切可能违反数据完整性的操作
 *          -悲观锁的实现，往往依靠底层提供的锁机制
 *          -悲观锁会导致其他所有需要锁的线程挂起，等待持有锁的线程释放锁
 *
 *        乐观锁：
 *          -假设不会发生并发冲突，每次不加锁而是假设没有冲突而去完成某项操作，只是在提交操作时，检查是否违反数据完整性
 *          -如果因为冲突失败就重试，直到成功为止
 *          -乐观锁大多是给予数据版本记录机制实现
 *          -为数据增加一个版本标识，比如在基于数据库表的版本解决方案中，一般是通过为数据库表增加一个“version”字段来实现。
 *           读取数据时，连版本号一起读出，之后更新时，对此版本号加一
 *          -乐观锁的缺点是不能解决脏读的问题
 *          -在实际生产环境里边，如果并发量不大且不允许脏读，可以使用悲观锁解决并发问题
 *          -如果系统的并发非常大的话，悲观锁定会带来非常大的性能问题，此时可以考虑选用乐观锁
 *
 */

/**
 * @brief CAS (Compare & Set / Compare & Swap)
 *
 * @note  CAS是解决多线程并行时，使用锁造成的性能损耗的一种机制
 *          -CAS包括三个操作数，内存位置（V）  预期原值（A）  新值（B）
 *          -如果内存位置的值与预期原值相匹配，那么处理器会自动将该位置的值更新为新值（若匹配，则说明没有其他线程改变这个内存位置的值）
 *          -否则，处理器不做任何操作
 *          -无论有哪种情况，它都会在CAS指令之前返回该位置的值
 *          -CAS有效的说明了“我认为位置V应该包含A；如果包含该值，则讲B放到这个位置；否则，不要更改该位置，只要告诉我这个位置现在的值即可”
 *
 *  @details
 * 一个CAS涉及的操作如下（假设内存中的原数据V，旧的预期值A，需要修改的新值B）：
 *              -比较A与V是否相等
 *              -如果比较相等，将B写入V
 *              -返回操作是成功
 */

#define SIMPLE_CAS 0x01
#define LIST_CAS (SIMPLE_CAS + 1)
#define QUEUE_CAS (SIMPLE_CAS + 2)

#define CAS_OPTION QUEUE_CAS

template <class T = int>
bool compare_and_swap(T *accum, T *dest, T newval) {
    if (*accum == *dest)  //  当前的等于预期的
    {
        *accum = newval;
        return true;
    }
    // *dest = newval;  // 进while时在做
    return false;
}

#if defined CAS_OPTION && (CAS_OPTION == SIMPLE_CAS)
int main() {
    spdlog::info("test for CAS");

    std::vector<std::thread> thread_pool;
    int tmep = 0;
    constexpr int kMaxThreadsNumber = 10;

    for (uint8_t i = 0; i < kMaxThreadsNumber; i++) {
        thread_pool.push_back(std::thread([&tmep]() {
            std::thread::id thread_id = std::this_thread::get_id();

            int old_data = 0;
            for (int i = 0; i < 100; i++) {
                old_data = tmep;
                while (__sync_bool_compare_and_swap(&tmep, old_data,
                                                    (tmep + 1)) == false) {
                    old_data = tmep;
                }
                // std::cout << thread_id << std::endl;
                spdlog::info("tmp:{}", tmep);
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }

            if (true) {
                std::cout << ++thread_end_count << std::endl;
            }
        }));
    }

    for (auto &tmp : thread_pool) {
        if (tmp.joinable()) {
            tmp.join();
        }
    }

    pause();
}
#elif defined CAS_OPTION && (CAS_OPTION == LIST_CAS)

constexpr int list_max_size = 100;

template <class ListType, int ListSize = list_max_size>
class Stack {
private:
    typedef struct Node {
        /* data */
        ListType data;
        Node *next;
        Node(const ListType &d) : data(d), next(nullptr) {}
    } Node;

    Node *top;  // top always point Stack Top
    int stack__max_size;

public:
    Stack() : top(nullptr), stack__max_size(ListSize){};
    void push(const ListType &data) {
        Node *tmp = new Node(data);

        do {
            tmp->next = top;
            /*
            当多个线程调用push，往栈顶推数据，这时就会出发CAS，保证同一时间内只有一个
            线程可以执行压栈操作，这里可能会产生ABA问题，如果有ABA冲突的可能，需要在
            加入一个版本号信息
            */
        } while (__sync_bool_compare_and_swap(&top, tmp->next, tmp) == false);
    }

    std::tuple<bool, ListType> pop() {
        auto return_tmp = std::make_tuple(false, 0);
        Node *tmp = nullptr;
        do {
            tmp = top;
            if (tmp == nullptr) {
                // spdlog::error("this stack is empty !!!");
                goto POP_END;
            }

        } while (__sync_bool_compare_and_swap(&top, tmp, tmp->next) == false);

        delete[] tmp;
        return_tmp = {true, tmp->data};
    POP_END:
        return return_tmp;
    }

    int ShowStackSize() const {
        int tmp_size = 0;

        Node *tmp = top;

        while (tmp != nullptr) {
            tmp = tmp->next;
            tmp_size++;
        }

        return tmp_size;
    }
};

std::once_flag push_log;
std::once_flag pop_log;

int main() {
    spdlog::info("practice lock free list");

    Stack<int> stack;

    std::vector<std::thread> thread_pool;

    for (int i = 0; i < 10; i++) {
        thread_pool.push_back(std::thread([&stack]() {
            for (int j = 0; j < 100; j++) {
                stack.push(j);
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }

            std::call_once(push_log, [&stack]() {
                spdlog::info("push stack_size:{}", stack.ShowStackSize());
            });

            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            for (int j = 0; j < 50; j++) {
                stack.pop();
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }

            std::call_once(pop_log, [&stack]() {
                spdlog::info("push stack_size:{}", stack.ShowStackSize());
            });
        }));
    }

    for (auto &tmp : thread_pool) {
        if (tmp.joinable()) {
            tmp.join();
        }
    }

    pause();
}

#elif defined CAS_OPTION && (CAS_OPTION == QUEUE_CAS)
enum class SendMsgErrorType {
    kNoError = 0,

    kQueueAlreadyFull = 1,
};
enum class ReceiveMsgErrorType {
    kNoError = 0,

    kQueueAlreadyEmpty = 1,
};

volatile std::atomic<uint32_t> count_timer{0};
template <class MsgType, int MsgSize = 10>
class QueueForMsg {
    struct QueueMsg {
        using QueueType = MsgType;
        using QueuePtr = MsgType *;
        using QueueConstPtr = const MsgType *;

        uint32_t queue_size;

        std::deque<QueueType> data;

        QueueMsg() : queue_size(0) {
            // data.resize(MsgSize);
            spdlog::info("Create Msg Queue , the max size is {}", MsgSize);
        };
    };

private:
    QueueMsg queue_msg;

    std::mutex queue_lock;

    std::condition_variable send_lock;
    std::condition_variable receive_lock;

public:
    /**
     * @brief  Send Msg Data to Msg Queue with time out,
     *         with use with ReceiveMsgFromQueueTail (Stack)
     *         with use with ReceiveMsgFromQueueHead (Queue)
     *
     * @param  msg send a msg to QueueMsg
     * @param  timeout /ms,0 means non-block
     * @return SendMsgErrorType error code
     */
    SendMsgErrorType SendMsgToQueue(const MsgType &msg,
                                    uint64_t timeout = 0) noexcept {
        SendMsgErrorType return_tmp = SendMsgErrorType::kNoError;

        std::unique_lock<std::mutex> lk(queue_lock);
        if (send_lock.wait_for(lk, std::chrono::milliseconds(timeout), [&]() {
                return (this->queue_msg.queue_size < MsgSize);
            })) {
            this->queue_msg.data.push_back(std::move(msg));
            ++queue_msg.queue_size;

            lk.unlock();
            receive_lock.notify_one();
        } else {
            // conditions are not met , time out
            return_tmp = SendMsgErrorType::kQueueAlreadyFull;
        }

        return return_tmp;
    }
    /**
     * @brief  Receive Msg Data to Msg Queue with time out,
     *         with use with SendMsgToQueue,
     *         is similar to Stack
     *
     * @param  msg receive msg from QueueMsg
     * @param  timeout /ms,0 means non-block
     * @return ReceiveMsgErrorType error code
     */
    ReceiveMsgErrorType ReceiveMsgFromQueueTail(MsgType *msg,
                                                uint64_t timeout = 0) noexcept {
        ReceiveMsgErrorType return_tmp = ReceiveMsgErrorType::kNoError;

        std::unique_lock<std::mutex> lk(queue_lock);
        if (receive_lock.wait_for(
                lk, std::chrono::milliseconds(timeout),
                [&]() { return (this->queue_msg.queue_size > 0); })) {
            *msg = this->queue_msg.data.back();
            this->queue_msg.data.pop_back();

            --queue_msg.queue_size;

            lk.unlock();
            send_lock.notify_one();
        } else {
            // conditions are not met , time out
            return_tmp = ReceiveMsgErrorType::kQueueAlreadyEmpty;
        }
        return return_tmp;
    }

    /**
     * @brief  Receive Msg Data to Msg Queue with time out,
     *         with use with SendMsgToQueue,
     *         is similar to Queue
     *
     * @param  msg receive msg from QueueMsg
     * @param  timeout /ms,0 means non-block
     * @return ReceiveMsgErrorType error code
     */
    ReceiveMsgErrorType ReceiveMsgFromQueueHead(MsgType *msg,
                                                uint64_t timeout = 0) noexcept {
        ReceiveMsgErrorType return_tmp = ReceiveMsgErrorType::kNoError;

        std::unique_lock<std::mutex> lk(queue_lock);
        if (receive_lock.wait_for(
                lk, std::chrono::milliseconds(timeout),
                [&]() { return (this->queue_msg.queue_size > 0); })) {
            *msg = this->queue_msg.data.front();
            this->queue_msg.data.pop_front();

            --queue_msg.queue_size;

            lk.unlock();
            send_lock.notify_one();
        } else {
            return_tmp = ReceiveMsgErrorType::kQueueAlreadyEmpty;
        }
        return return_tmp;
    }

public:
    QueueForMsg() = default;
    ~QueueForMsg() = default;

private:
    QueueForMsg(const QueueForMsg &);
    QueueForMsg &operator=(const QueueForMsg &);
};

template <class MsgType>
class QueueForMsg<MsgType, 1> {
private:
    inline bool SpinSynchronousWithTimeOut() {
        uint32_t error_counts = 0;
        auto old_data = identification_count;
        while (__sync_bool_compare_and_swap(&identification_count, old_data,
                                            (identification_count + 1)) ==
               false) {
            if (++error_counts >= error_times) {
                return false;
            }
            old_data = identification_count;
        }
        return true;
    }

public:
    void SendMsgToQueueWithOverWrite(const MsgType &msg) {
        if (SpinSynchronousWithTimeOut()) {
            data.at(0) = msg;
        } else {
            spdlog::error(
                "Please Check if call PeekMsgFromQueue to receive msg");
            std::abort();
        }
    }

    void PeekMsgFromQueue(MsgType *msg) {
        if (SpinSynchronousWithTimeOut()) {
            *msg = data.at(0);
        } else {
            spdlog::error(
                "Please Check if call SendMsgToQueueWithOverWrite to send msg");
            std::abort();
        }
    }

private:
    std::array<MsgType, 1> data;

    uint64_t identification_count;

    static constexpr uint8_t error_times = 10;

public:
    QueueForMsg() : identification_count(0) {
        spdlog::info("Single Msg Queue is Created");
    };
    ~QueueForMsg() = default;

private:
    QueueForMsg(const QueueForMsg &);
    QueueForMsg &operator=(const QueueForMsg &);
};

template <class ClassType>
void SendSensorMsgToQueue(ClassType &msg) {
    for (int32_t i = 0; i < 1000;) {
        msg.SendMsgToQueueWithOverWrite(i);
        if (1) {
            // spdlog::info("pop {} from the queue", tmp);
            spdlog::info("Tail count_timer:{},{}", count_timer++, i++);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
}

template <class ClassType>
void DealSensorMsgFromQueueTail(ClassType &msg) {
    while (1) {
        int32_t tmp;
        msg.PeekMsgFromQueue(&tmp);
        if (1) {
            // spdlog::info("pop {} from the queue", tmp);
            spdlog::info("Tail count_timer:{},{}", count_timer++, tmp);
            if (tmp == 999) {
                break;
            }
        }

        // ReceiveMsgErrorType error_code = msg.ReceiveMsgFromQueueTail(&tmp,
        // 100); if (error_code == ReceiveMsgErrorType::kNoError)
        // {
        //     // spdlog::info("pop {} from the queue", tmp);
        //     spdlog::info("Tail count_timer:{},{}", count_timer++, tmp);
        // }
    }
}

template <class ClassType>
void DealSensorMsgFromQueueHead(ClassType &msg) {
    while (1) {
        int32_t tmp = 0;
        ReceiveMsgErrorType error_code = msg.ReceiveMsgFromQueueHead(&tmp, 100);
        if (error_code == ReceiveMsgErrorType::kNoError) {
            // spdlog::info("pop {} from the queue", tmp);
            spdlog::info("Head count_timer:{},{}", count_timer++, tmp);
        }
    }
}

int main() {
    spdlog::info("Copy And Swap For QueueMsg");

#if 0
    constexpr auto kQueueMsgMaxSize = 100;
    using QueueTypeInt = QueueForMsg<int, kQueueMsgMaxSize>;

    QueueTypeInt queue_odometer;
    std::vector<std::thread> thread_pool;

    thread_pool.emplace_back(std::thread(DealSensorMsgFromQueueTail<QueueTypeInt>, std::ref(queue_odometer)));
    thread_pool.emplace_back(std::thread(SendSensorMsgToQueue<QueueTypeInt>, std::ref(queue_odometer)));
    // thread_pool.emplace_back(std::thread(DealSensorMsgFromQueueHead<QueueTypeInt>, std::ref(queue_odometer)));

    for (auto &tmp : thread_pool)
    {
        if (tmp.joinable())
        {
            tmp.join();
        }
    }
#else

    using QueueTypeInt = QueueForMsg<int, 1>;

    QueueTypeInt queue_odometer;

    std::vector<std::thread> thread_pool;
    thread_pool.emplace_back(
        std::thread(DealSensorMsgFromQueueTail<QueueForMsg<int, 1> >,
                    std::ref(queue_odometer)));
    thread_pool.emplace_back(std::thread(SendSensorMsgToQueue<QueueTypeInt>,
                                         std::ref(queue_odometer)));

    for (auto &tmp : thread_pool) {
        if (tmp.joinable()) {
            tmp.join();
        }
    }

    // queue_odometer.SendMsgToQueueWithOverWrite(100);

#endif

    pause();
}

#endif