/*
 *  @file   Delay.h
 */

#ifndef _DELAY_H_
#define _DELAY_H_

#include "TaskInterface.h"
#include "Scheduler.h"

#include <chrono>
#include <coroutine>

class DelayTask : public TaskInterface, public std::enable_shared_from_this<DelayTask> {
    int ms;
    std::chrono::high_resolution_clock::time_point startTime;
    std::coroutine_handle<> handle;
    bool started = false;

public:
    explicit DelayTask(int milliseconds) : ms(milliseconds) {}

    bool await_ready() const noexcept { return false; }

    void await_suspend(std::coroutine_handle<> h) {
        handle = h;
        startTime = std::chrono::high_resolution_clock::now();
        started = true;
        Scheduler::Instance().Add(shared_from_this());
    }

    void await_resume() noexcept {}

    bool IsDone() const override {
        if (!started) return false;
        auto now = std::chrono::high_resolution_clock::now();
        return std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime).count() >= ms;
    }

    void Resume() override {
        if (handle && IsDone()) {
            auto h = handle;
            handle = nullptr;
            h.resume();
        }
    }
};

// Delay ä÷êî
inline auto Delay(int ms) {
    struct Awaitable {
        std::shared_ptr<DelayTask> task;
        Awaitable(int t) : task(std::make_shared<DelayTask>(t)) {}
        bool await_ready() { return task->IsDone(); }
        void await_suspend(std::coroutine_handle<> h) { task->await_suspend(h); }
        void await_resume() {}
    };
    return Awaitable(ms);
}

#endif // !_DELAY_H_
