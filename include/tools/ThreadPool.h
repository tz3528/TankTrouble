#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include "pch.h"

#include <thread>
#include <queue>
#include <vector>
#include <functional>

class ThreadPool {
public:
	ThreadPool(int ThreadNumber);
	~ThreadPool();

	template<class F, class... Args>
	void addTask(F&& f, Args ... args) {
		std::function<void()> task =
			std::bind(std::forward<F>(f), std::forward<Args>(args)...);

		{
			std::unique_lock<std::mutex> lock(mutex);
			tasks.emplace(std::move(task));
		}

		condition.notify_one();
	}



private:
	std::vector<std::thread> threads;
	std::queue<std::function<void()>> tasks;

	std::mutex mutex;
	std::condition_variable condition;

	bool stop;
};

#endif
