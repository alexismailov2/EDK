/*******************************************************************************
 Copyright (C) 2018 Philips Lighting Holding B.V.
 All Rights Reserved.
 ********************************************************************************/

#include <algorithm>
#include <string>

#include "support/threading/ThreadPool.h"
#include "support/threading/Thread.h"

namespace support {

    ThreadPool::ThreadPool(size_t workers, bool finish_tasks_on_close, std::string name)
            : _close(false)
            , _should_finish_tasks_on_close(finish_tasks_on_close) {
        for (size_t i = 0; i < workers; i++) {
            const auto thread_name = name.empty() ? name : name + "-" + std::to_string(i + 1);
            _threads.emplace_back(std::make_unique<support::Thread>(thread_name, std::bind(&ThreadPool::event_loop, this)));
        }
    }

    void ThreadPool::event_loop() {
        std::unique_lock<std::mutex> tasks_lock(_tasks_mutex);
        while (should_execute_tasks()) {
            _tasks_condition.wait(tasks_lock, [&] () -> bool {
                // Wait until there is a new task or the thread pool closed
                return (!_tasks.empty() || _close);
            });

            if (should_execute_tasks()) {
                // Get task from the queue
                ThreadPoolTask task = _tasks.front();
                // Remove task from the queue
                _tasks.pop();
                tasks_lock.unlock();

                // Execute the task
                call_and_ignore_exception(task);
                // Destroy the task out of critical section otherwise we can run to deadlock.
                task = nullptr;

                tasks_lock.lock();
            }
        }
    }
    
    ThreadPool::~ThreadPool() {
        shutdown();
    }

    void ThreadPool::shutdown() {
        {
            std::lock_guard<std::mutex> tasks_lock(_tasks_mutex);
            _close = true;
            _tasks_condition.notify_all();
        }

        for (auto&& worker : _threads) {
            worker->join();
        }
        _threads.clear();
    }
    
    bool ThreadPool::should_execute_tasks() const {
        return !_close || (!_tasks.empty() && _should_finish_tasks_on_close);
    }
}  // namespace support
