#ifndef TASK_H
#define TASK_H

#include <assert.h>
#include <functional>
#include <future>

#include "ThreadPoolExecutor.h"

using Scheduler = ThreadPoolExecutor::Scheduler;

template<typename Result_t>
class Task
{
public:
    Task(std::function<Result_t()>&& func, ThreadPoolExecutor::Scheduler sched = ThreadPoolExecutor::Scheduler::Default) noexcept
        : _firstFunc{std::move(func)}, _firstScheduler{sched}
    { }
    Task(const Task&) = delete;
    Task(Task&&) = default;

    Task& operator=(const Task&) = delete;
    Task& operator=(Task&&) = default;

    Task& then(std::function<void(std::future<Result_t>)>&& func)
    {
        _thenFunc = std::move(func);
        return *this;
    }

    void start()
    {
        if (_firstFunc == nullptr || _thenFunc == nullptr) {
            // Task<> does not support reusing a single instance and
            // Task<>::then(...) has to be called.
            assert(_firstFunc != nullptr && _thenFunc != nullptr);
            return;
        }

        auto firstFunc = std::move(_firstFunc);
        auto thenFunc = std::move(_thenFunc);
        auto thenSched = _thenScheduler;

        _firstFunc = nullptr;
        _thenFunc = nullptr;

        ThreadPoolExecutor::addTask([firstFunc, thenFunc, thenSched] {
            auto promise = std::make_shared<std::promise<Result_t>>();

            try {
                auto result = firstFunc();
                promise->set_value(std::move(result));
            } catch (...) {
                try {
                    promise->set_exception(std::current_exception());
                } catch (...) { }
            }

            ThreadPoolExecutor::addTask([thenFunc, promise] {
                thenFunc(promise->get_future());
            }, thenSched);
        }, _firstScheduler);
    }

private:
    using Scheduler = ThreadPoolExecutor::Scheduler;

    std::function<Result_t()> _firstFunc{};
    std::function<void(std::future<Result_t>)> _thenFunc{};
    Scheduler _firstScheduler{Scheduler::Default};
    Scheduler _thenScheduler{Scheduler::Main};
};

template<>
class Task<void>
{
public:
    Task(std::function<void()>&& func, ThreadPoolExecutor::Scheduler sched = ThreadPoolExecutor::Scheduler::Default) noexcept
        : _firstFunc{std::move(func)}, _firstScheduler{sched}
    { }
    Task(const Task&) = delete;
    Task(Task&&) = default;

    Task& operator=(const Task&) = delete;
    Task& operator=(Task&&) = default;

    Task& then(std::function<void(std::future<void>)>&& func, ThreadPoolExecutor::Scheduler sched = ThreadPoolExecutor::Scheduler::Main) noexcept
    {
        _thenFunc = std::move(func);
        _thenScheduler = sched;
        return *this;
    }

    Task& then(std::function<void()>&& func, ThreadPoolExecutor::Scheduler sched = ThreadPoolExecutor::Scheduler::Main)
    {
        _thenFunc = [func](std::future<void>) { func(); };
        _thenScheduler = sched;
        return *this;
    }

    void start()
    {
        if (_firstFunc == nullptr) {
            // Task<> does not support reusing a single instance.
            assert(_firstFunc != nullptr);
            return;
        }

        auto firstFunc = std::move(_firstFunc);
        auto thenFunc = std::move(_thenFunc);
        auto thenSched = _thenScheduler;

        _firstFunc = nullptr;
        _thenFunc = nullptr;

        ThreadPoolExecutor::addTask([firstFunc, thenFunc, thenSched] {
            auto promise = std::make_shared<std::promise<void>>();

            try {
                firstFunc();
                promise->set_value();
            } catch (...) {
                try {
                    promise->set_exception(std::current_exception());
                } catch (...) { }
            }

            if (thenFunc == nullptr)
                return;

            ThreadPoolExecutor::addTask([thenFunc, promise] {
                thenFunc(promise->get_future());
            }, thenSched);
        }, _firstScheduler);
    }

private:
    using Scheduler = ThreadPoolExecutor::Scheduler;

    std::function<void()> _firstFunc{};
    std::function<void(std::future<void>)> _thenFunc{};
    Scheduler _firstScheduler{Scheduler::Default};
    Scheduler _thenScheduler{Scheduler::Main};
};

#endif // TASK_H
