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
        Task<T>* get_return_object() {
            return reinterpret_cast<Task<T>*>(this);
        }
        std::suspend_always initial_suspend() { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        void return_void() {}
        void unhandled_exception() {}
    };

    using handle_type = std::coroutine_handle<promise_type>;
    handle_type handle;

    Task(handle_type h) : handle(h) {}

    void Resume() { if (handle && !handle.done()) handle.resume(); }
    bool IsDone() const { return !handle || handle.done(); }
};

// voidêÍóp
// Task.h
// TaskVoid.h
class TaskVoid : public TaskInterface, public std::enable_shared_from_this<TaskVoid> {
public:
    struct promise_type {
        TaskVoid get_return_object() {
            auto task = std::make_shared<TaskVoid>();
            task->handle = std::coroutine_handle<promise_type>::from_promise(*this);
            Scheduler::Instance().Add(task);
            return *task;
        }
        std::suspend_always initial_suspend() { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        void return_void() {}
        void unhandled_exception() {}
    };

    std::coroutine_handle<promise_type> handle;

    bool IsDone() const override { return !handle || handle.done(); }

    void Resume() override {
        if (handle && !handle.done()) handle.resume();
    }
}; 
#endif // !1_TASK_H_