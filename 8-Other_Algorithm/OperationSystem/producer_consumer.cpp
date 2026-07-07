#include <iostream>
#include <thread>
#include <semaphore>
#include <vector>
#include <chrono>
#include <random>
#include <queue>

// ==========================================
// 生产者-消费者问题 —— 全部使用 std::counting_semaphore
// ==========================================

const int BUFFER_SIZE = 5;       // 缓冲区容量
const int PRODUCER_COUNT = 3;    // 生产者数量
const int CONSUMER_COUNT = 2;    // 消费者数量
const int ITEMS_PER_PRODUCER = 4; // 每个生产者生产几件

std::queue<int> buffer;          // 共享缓冲区

// 三个信号量：
//   empty: 缓冲区空位数，初值 = BUFFER_SIZE
//   full:  缓冲区已填充数，初值 = 0
//   mutex: 互斥访问缓冲区，初值 = 1（二元信号量）
std::counting_semaphore<BUFFER_SIZE> empty(BUFFER_SIZE);
std::counting_semaphore<BUFFER_SIZE> full(0);
std::counting_semaphore<1> mutex(1);

// 打印互斥信号量
std::counting_semaphore<1> print_sem(1);

// RAII 包装
struct SemLock {
    std::counting_semaphore<1>& sem;
    explicit SemLock(std::counting_semaphore<1>& s) : sem(s) { sem.acquire(); }
    ~SemLock() { sem.release(); }
};

// 模拟生产一件产品耗时
void produce_item(int id, int item_no) {
    static thread_local std::mt19937_64 eng{std::random_device{}()};
    std::uniform_int_distribution<> dist{200, 800};
    std::this_thread::sleep_for(std::chrono::milliseconds(dist(eng)));

    {
        SemLock lock(print_sem);
        std::cout << "[生产] 生产者 " << id << " 生产了产品 " << item_no << std::endl;
    }
}

// 模拟消费一件产品耗时
void consume_item(int id, int item) {
    static thread_local std::mt19937_64 eng{std::random_device{}()};
    std::uniform_int_distribution<> dist{300, 1000};
    std::this_thread::sleep_for(std::chrono::milliseconds(dist(eng)));

    {
        SemLock lock(print_sem);
        std::cout << "  <--- [消费] 消费者 " << id << " 消费了产品 " << item << std::endl;
    }
}

// 生产者线程
void producer(int id) {
    for (int i = 1; i <= ITEMS_PER_PRODUCER; i++) {
        // 生产产品（耗时操作，放在临界区外）
        produce_item(id, i);

        // P(empty)：等待空位
        empty.acquire();

        // P(mutex)：进入临界区
        mutex.acquire();
        buffer.push(id * 100 + i);  // 产品编号 = 生产者id*100 + 序号
        {
            SemLock lock(print_sem);
            std::cout << "  >>> [放入] 生产者 " << id
                      << " 将产品 " << (id * 100 + i)
                      << " 放入缓冲区 (当前 " << buffer.size() << " 件)"
                      << std::endl;
        }
        // V(mutex)：离开临界区
        mutex.release();

        // V(full)：增加一个已填充位置
        full.release();
    }
}

// 消费者线程
void consumer(int id) {
    // 每个消费者消费的总次数 = (生产者总数 * 每生产者产量) / 消费者数量
    int total_items = (PRODUCER_COUNT * ITEMS_PER_PRODUCER) / CONSUMER_COUNT;
    for (int j = 0; j < total_items; j++) {
        // P(full)：等待有产品可取
        full.acquire();

        // P(mutex)：进入临界区
        mutex.acquire();
        int item = buffer.front();
        buffer.pop();
        {
            SemLock lock(print_sem);
            std::cout << "  <<< [取出] 消费者 " << id
                      << " 从缓冲区取出产品 " << item
                      << " (当前 " << buffer.size() << " 件)"
                      << std::endl;
        }
        // V(mutex)：离开临界区
        mutex.release();

        // V(empty)：增加一个空位
        empty.release();

        // 消费产品（耗时操作，放在临界区外）
        consume_item(id, item);
    }
}

int main() {
    std::vector<std::thread> threads;

    std::cout << "========== 生产者-消费者模拟开始 ==========" << std::endl;
    std::cout << "缓冲区容量: " << BUFFER_SIZE
              << "  生产者: " << PRODUCER_COUNT
              << "  消费者: " << CONSUMER_COUNT << std::endl;
    std::cout << "-------------------------------------------" << std::endl;

    // 创建生产者线程
    for (int i = 1; i <= PRODUCER_COUNT; i++) {
        threads.push_back(std::thread(producer, i));
    }

    // 创建消费者线程
    for (int i = 1; i <= CONSUMER_COUNT; i++) {
        threads.push_back(std::thread(consumer, i));
    }

    // 等待所有线程结束
    for (auto& t : threads) {
        t.join();
    }

    std::cout << "-------------------------------------------" << std::endl;
    std::cout << "========== 所有生产消费完毕 ==========" << std::endl;

    return 0;
}
