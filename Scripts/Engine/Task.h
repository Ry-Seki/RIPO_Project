/*
 *  @file   Task.h
 */
#ifndef _TASK_H_
#define _TASK_H_

#include "TaskInterface.h"
#include "Scheduler.h"
#include <coroutine>
#include <memory>

template<typename T = void>
class Task : public TaskInterface, public std::enable_shared_from_this<Task<T>> {
public:
    struct promise_type {
        T value;
        std::exception_ptr exception;

        Task get_return_object() {
            auto t = std::make_shared<Task<T>>();
            t->handle = std::coroutine_handle<promise_type>::from_promise(*this);
            Scheduler::Instance().Add(t); // é©ìÆìoò^
            return *t;
        }

        std::suspend_always initial_suspend() { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }

        // T != void óp
        void return_value(T v) { value = v; }

        void unhandled_exception() { exception = std::current_exception(); }
    };

    std::coroutine_handle<promise_type> handle;

    bool IsDone() const override { return !handle || handle.done(); }
    void Resume() override { if (handle) handle.resume(); }
};

// void êÍóp
template<>
class Task<void> : public TaskInterface, public std::enable_shared_from_this<Task<void>> {
public:
    struct promise_type {
        std::exception_ptr exception;

        Task get_return_object() {
            auto t = std::make_shared<Task<void>>();
            t->handle = std::coroutine_handle<promise_type>::from_promise(*this);
            Scheduler::Instance().Add(t);
            return *t;
        }

        std::suspend_always initial_suspend() { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }

        void return_void() {}
        void unhandled_exception() { exception = std::current_exception(); }
    };

    std::coroutine_handle<promise_type> handle;

    bool IsDone() const override { return !handle || handle.done(); }
    void Resume() override { if (handle) handle.resume(); }
};

#endif // !1_TASK_H_