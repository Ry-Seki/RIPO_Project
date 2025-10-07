/*
 *  @file   Delay.h
 */

#ifndef _DELAY_H_
#define _DELAY_H_

#include "TaskInterface.h"
#include "Scheduler.h"

#include <chrono>
#include <coroutine>

class Delay : public TaskInterface, public std::enable_shared_from_this<Delay> {
private:
    std::chrono::high_resolution_clock::time_point startTime;
    std::chrono::milliseconds duration;
    std::coroutine_handle<> handle;

public:
    explicit Delay(int setTime)
        : duration(setTime) {
        startTime = std::chrono::high_resolution_clock::now();
    }

    bool await_ready() const noexcept { return false; }

    void await_suspend(std::coroutine_handle<> h) {
        handle = h;
        Scheduler::Instance().Add(shared_from_this());
    }

    void await_resume() noexcept {}

    // TaskInterface
    bool IsDone() const override {
        auto now = std::chrono::high_resolution_clock::now();
        return now - startTime >= duration;
    }

    void Resume() override {
        if (IsDone() && handle) handle.resume();
    }
};
#endif // !_DELAY_H_
