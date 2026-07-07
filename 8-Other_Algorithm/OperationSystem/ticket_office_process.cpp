#include <iostream>
#include <unistd.h>       // fork, getpid
#include <sys/wait.h>     // waitpid
#include <sys/mman.h>     // mmap, shm_open
#include <fcntl.h>        // O_CREAT, O_RDWR
#include <semaphore.h>    // POSIX 信号量
#include <cstring>        // strerror
#include <cerrno>         // errno
#include <chrono>
#include <random>
#include <thread>

// ==========================================
// 多进程版：售票厅信号量模拟
// 关键点：fork() 后父子进程不共享内存，
//         因此信号量必须放在“共享内存”中
// ==========================================

struct SharedData {
    sem_t S;            // 信号量（售票厅容量为 20）
    sem_t print_sem;    // 保护控制台打印的信号量（相当于之前的 print_mtx）
};

int main() {
    const int CAPACITY = 20;       // 售票厅容量
    const int TOTAL_BUYERS = 30;   // 购票者总数

    // ---------- 1. 在共享内存中创建信号量 ----------
    // 因为 fork() 之后父子进程地址空间隔离，
    // 信号量必须放在共享内存区域，才能跨进程共享。
    SharedData* shared = (SharedData*)mmap(
        nullptr,
        sizeof(SharedData),
        PROT_READ | PROT_WRITE,
        MAP_SHARED | MAP_ANONYMOUS,  // MAP_SHARED 确保父子进程看到同一块内存
        -1, 0
    );

    sem_init(&shared->S,       1, CAPACITY);  // pshared=1 → 进程间共享
    sem_init(&shared->print_sem, 1, 1);       // 打印互斥信号量，初值=1

    std::cout << "车站售票厅开门啦！容量: " << CAPACITY
              << " 人，当前来了 " << TOTAL_BUYERS << " 人。" << std::endl;
    std::cout << "---------------------------------------------------------" << std::endl;

    // ---------- 2. fork() 创建多个子进程 ----------
    for (int i = 1; i <= TOTAL_BUYERS; i++) {
        pid_t pid = fork();

        if (pid == 0) {
            // ========== 子进程：模拟购票者 ==========
            int id = i;  // 每个子进程记住自己的 id

            // 模拟到达
            sem_wait(&shared->print_sem);
            std::cout << "[到达] 购票者 " << id << " (PID=" << getpid()
                      << ") 到达门外。" << std::endl;
            sem_post(&shared->print_sem);

            // ====== P(S)：申请进入售票厅 ======
            sem_wait(&shared->S);
            // ====== 进入临界区 ======

            sem_wait(&shared->print_sem);
            std::cout << "  ---> [进入] 购票者 " << id << " 进入售票厅，正在购票..." << std::endl;
            sem_post(&shared->print_sem);

            // 模拟购票耗时（随机 100~500ms）
            std::mt19937_64 eng{std::random_device{}()};
            std::uniform_int_distribution<> dist{100, 500};
            std::this_thread::sleep_for(std::chrono::milliseconds(dist(eng)));

            sem_wait(&shared->print_sem);
            std::cout << "  <--- [离开] 购票者 " << id << " 购票完毕，离开售票厅。" << std::endl;
            sem_post(&shared->print_sem);

            // ====== V(S)：离开售票厅 ======
            sem_post(&shared->S);

            // 子进程完成任务，退出
            _exit(0);  // 注意：用 _exit 而不是 exit，避免冲刷父进程的 IO 缓冲区
        }
    }

    // ---------- 3. 父进程等待所有子进程结束 ----------
    // 相当于多线程版的 join()
    for (int i = 0; i < TOTAL_BUYERS; i++) {
        waitpid(-1, nullptr, 0);  // -1 表示等待任意一个子进程
    }

    std::cout << "---------------------------------------------------------" << std::endl;
    std::cout << "所有人购票完毕，系统结束。" << std::endl;

    // ---------- 4. 清理资源 ----------
    sem_destroy(&shared->S);
    sem_destroy(&shared->print_sem);
    munmap(shared, sizeof(SharedData));

    return 0;
}
