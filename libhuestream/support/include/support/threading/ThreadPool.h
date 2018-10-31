/*******************************************************************************
 Copyright (C) 2018 Philips Lighting Holding B.V.
 All Rights Reserved.
 ********************************************************************************/

#pragma once

#include <atomic>
#include <condition_variable>
#include <functional>
#include <future>
#include <memory>
#include <vector>
#include <mutex>
#include <string>
#include <queue>
#include <type_traits>
#include <utility>

#include "support/util/VectorOperations.h"
#include "support/util/Provider.h"

#define FUNCTION_RETURN_TYPE(Function, Args) typename std::result_of<Function(Args...)>::type

    namespace support {

    class Thread;

    using ThreadPoolTask = std::function<void()>;

    class ThreadPool final {
    public:
        /**
         @param finish_tasks_on_close on close, all tasks are executed before exiting
         @param name thread pool name applied to the workers
         */
        explicit ThreadPool(size_t workers, bool finish_tasks_on_close = false, std::string name = {});

        /**
         Will block until the thread pool is closed
         */
        virtual ~ThreadPool();

        template<typename Function, typename... Args>
        std::future<FUNCTION_RETURN_TYPE(Function, Args)> add_task(Function&& function, Args&&... args) {
            std::future<FUNCTION_RETURN_TYPE(Function, Args)> future;

            typedef std::packaged_task<FUNCTION_RETURN_TYPE(Function, Args)()> TaskAsync;

            std::unique_lock<std::mutex> tasks_lock(_tasks_mutex);

            if (!_close) {
                std::shared_ptr<TaskAsync> task_async = std::make_shared<TaskAsync>(
                        std::bind(std::forward<Function>(function), std::forward<Args>(args)...));
                future = task_async->get_future();

                _tasks.push([task_async] () -> void {
                    (*task_async)();
                });

                _tasks_condition.notify_one();
            }

            return future;
        }

    protected:
        /**
         thread pool thread main even_loop
         */
        void event_loop();

        /**
        Check whether tasks should be executed.
        Conditions:
        - close() or destructor has not been called
        OR:
        - threadpool is configured that all tasks should be executed on closing
        - tasks list is not empty
        */
        bool should_execute_tasks() const;

        /**
         Shutdown thread pool. After calling this method object is in invalid state,
         so it must be called just from the class destructor.
         */
        void shutdown();

    private:
        std::queue<ThreadPoolTask> _tasks;
        std::vector<std::unique_ptr<Thread>>        _threads;
        std::mutex                 _tasks_mutex;
        std::condition_variable    _tasks_condition;
        std::atomic<bool>          _close;
        bool                       _should_finish_tasks_on_close;
    };

    using GlobalThreadPool = Provider<std::shared_ptr<ThreadPool>>;
}  // namespace support

template<>
struct default_object<std::shared_ptr<support::ThreadPool>> {
    static std::shared_ptr<support::ThreadPool> get() {
        return std::make_shared<support::ThreadPool>(4, false, "global-tp");
    }
};