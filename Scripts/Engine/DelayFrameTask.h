/*
 *	@file	DelayFrameTask.h
 */

#ifndef _DELAYFRAMETASK_H_
#define _DELAYFRAMETASK_H_

#include "Scheduler.h"
#include "TaskInterface.h"

#include <coroutine>
#include <memory>

class DelayFrameTask : public TaskInterface, public std::enable_shared_from_this<DelayFrameTask> {
private:
    int frames;
    std::coroutine_handle<> handle;

public:
    explicit DelayFrameTask(int f) : frames(f) {}

    bool await_ready() const noexcept { return frames <= 0; }

    void await_suspend(std::coroutine_handle<> h) {
        handle = h;
        Scheduler::Instance().Add(shared_from_this());
    }

    void await_resume() noexcept {}

    // TaskInterface
    bool IsDone() const override { return frames <= 0; }

    void Resume() override {
        if (frames > 0) --frames;
        if (frames <= 0 && handle) handle.resume();
    }
};
#endif // !_DELAYFRAMETASK_H_

