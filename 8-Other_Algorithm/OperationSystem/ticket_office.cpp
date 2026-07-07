#include <iostream>
#include <thread>
#include <semaphore>
#include <vector>
#include <chrono>
#include <random>

// ==========================================
// 全部使用 std::counting_semaphore，统一机制
// ==========================================

// 打印互斥信号量（二元信号量 = 互斥锁），初值 1
std::counting_semaphore<1> print_sem(1);

// 售票厅信号量，容量 20
std::counting_semaphore<20> S(20);

// RAII 包装：让 counting_semaphore 也能像 lock_guard 一样自动释放
struct SemLock {
    std::counting_semaphore<1>& sem;
    explicit SemLock(std::counting_semaphore<1>& s) : sem(s) { sem.acquire(); }
    ~SemLock() { sem.release(); }
};

// 模拟购票耗时（随机 100~500ms）
void buy_ticket() {
    static thread_local std::mt19937_64 eng{std::random_device{}()};
    std::uniform_int_distribution<> dist{100, 500};
    std::this_thread::sleep_for(std::chrono::milliseconds(dist(eng)));
}

// 购票者线程的执行函数
void buyer_process(int id) {
    // 模拟到达
    {
        SemLock lock(print_sem);
        std::cout << "[到达] 购票者 " << id << " 到达门外，准备申请进入。" << std::endl;
    }

    // P(S)：申请进入售票厅
    S.acquire();

    // 进入售票厅
    {
        SemLock lock(print_sem);
        std::cout << "  ---> [进入] 购票者 " << id << " 进入售票厅，正在购票..." << std::endl;
    }

    // 模拟购票耗时
    buy_ticket();

    // 离开售票厅
    {
        SemLock lock(print_sem);
        std::cout << "  <--- [离开] 购票者 " << id << " 购票完毕，离开售票厅。" << std::endl;
    }

    // V(S)：离开，释放一个位置
    S.release();
}

// 主函数
int main() {
    int total_buyers = 30;
    std::vector<std::thread> threads;

    std::cout << "车站售票厅开门啦！容量: 20 人，当前来了 " << total_buyers << " 人。" << std::endl;
    std::cout << "---------------------------------------------------------" << std::endl;

    for (int i = 1; i <= total_buyers; i++) {
        threads.push_back(std::thread(buyer_process, i));
    }

    for (auto& t : threads) {
        t.join();
    }

    std::cout << "---------------------------------------------------------" << std::endl;
    std::cout << "所有人购票完毕，系统结束。" << std::endl;

    return 0;
}
