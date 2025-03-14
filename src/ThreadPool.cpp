#include "ThreadPool.h"

ThreadPool::ThreadPool(int ThreadNumber)
    :stop(false) {
    for (int i = 0;i < ThreadNumber;++i) {
        threads.emplace_back([this] {
            while (1) {
                std::unique_lock<std::mutex> lock(mutex);
                condition.wait(lock, [this] {
                    return !tasks.empty() || stop;
                    });

                if (tasks.empty() && stop) {
                    return;
                }

                std::function<void()> task = std::move(tasks.front());
                tasks.pop();

                lock.unlock();
                task();

            }

            });
    }
}

ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock(mutex);
        stop = true;

    }

    condition.notify_all();
    for (auto& thread : threads) {
        thread.join();
    }

}