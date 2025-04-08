#ifndef TIMER_MANAGER_HPP
#define TIMER_MANAGER_HPP

#include <set>
#include <unordered_map>
#include <functional>
#include <chrono>
#include <mutex>
#include <thread>
#include <condition_variable>

using std::set, std::unordered_map;
using namespace std::chrono;

/**
 * @brief 定时任务结构
 */
struct TimerTask {
    int id;
    steady_clock::time_point exec_time;
    std::function<int()> callback;
    bool operator<(const TimerTask& other) const {
        // 优先比较执行时间
        if (exec_time != other.exec_time) {
            return exec_time < other.exec_time;
        }
        return id < other.id;
    }
};

/**
 * @brief 定时任务管理器
 */
class TimerManager {
public:

    TimerManager() {
        work = std::thread(&TimerManager::run, this);
    };

    ~TimerManager() {
        {
            std::unique_lock<std::mutex> lock(mutex);
            running = false;

        }
        cv.notify_one();
        if (work.joinable()) {
            work.join();
        }
    }

    template<class F, class... Args>
    void addTask(int id, int exec_time, F&& f, Args ... args) {
        std::lock_guard<std::mutex> lock(mutex);
        auto [it, _] =tasks.insert(TimerTask{
            id, steady_clock::now() + milliseconds(exec_time),
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)});
        index[id] = it;
        cv.notify_one();
    }

    void erase(int id) {
        std::lock_guard<std::mutex> lock(mutex);
        if (auto it = index.find(id); it != index.end()) {
            tasks.erase(it->second);
            index.erase(it);
        }
    }
private:
    void run() {
        while (running) {
            std::unique_lock<std::mutex> lock(mutex);
            if (tasks.empty()) {
                cv.wait(lock);
                continue;
            }

            cv.wait_until(lock, tasks.begin()->exec_time);

            while (!tasks.empty()) {
                auto task = *tasks.begin();
                if (task.exec_time > steady_clock::now()) {
                    break;
                }
                tasks.erase(tasks.begin());

                lock.unlock();
                auto delay = task.callback();
                lock.lock();

                if (delay > 0) {
                    task.exec_time += milliseconds(delay);
                    auto [it,_] = tasks.insert(task);
                    index[task.id] = it;
                }
                else {
                    index.erase(task.id);
                }
            }
        }
    }

    std::thread work;

    set<TimerTask> tasks;
    unordered_map<int, set<TimerTask>::iterator> index;

    bool running = true;

    std::mutex mutex;
    std::condition_variable cv;
};

#endif
